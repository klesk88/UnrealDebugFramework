// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "BottomBar/Manager/KLDebugFrameworkBottomBarManager.h"

// modules
#include "User/Framework/Internal/BottomBar/StaticMemory/KLDebugBottomBarManager.h"
#include "User/Framework/Internal/BottomBar/StaticMemory/KLDebugBottomBarManagerEntryBase.h"
#include "User/Framework/Public/Window/BottomBar/KLDebugBottomBarInterface.h"
#include "User/Framework/Public/Window/BottomBar/KLDebugModeInterfaceAllInput.h"

// engine
#include "Engine/World.h"

void FKLDebugFrameworkBottomBarManager::Initialize()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugFrameworkBottomBatManager_Initialize);

    InitInternal<FKLDebugBottomBarManager, FKLDebugBottomBarManagerEntryBase>();

    mSortedBars.Reserve(mIDToData.Num());
    for (TMap<FName, uint32>::TConstIterator Iter(mIDToData.CreateConstIterator()); Iter; ++Iter)
    {
        const FName& Key = Iter.Key();
        IKLDebugBottomBarInterface& Interface = GetDataMutable(Iter.Value());
        mSortedBars.Emplace(Interface.GetFriendlyName(), Interface);
    }

    mSortedBars.Sort([](const FKLDebugFrameworkBottomBarSortedData& _Left, const FKLDebugFrameworkBottomBarSortedData& _Right) -> bool {
        return _Left.GetName() < _Right.GetName();
    });
}

void FKLDebugFrameworkBottomBarManager::Shutdown()
{
}

TUniquePtr<IKLDebugContextInterface> FKLDebugFrameworkBottomBarManager::UpdateBottomBarIfNeeded(const UWorld& _World, const int32 _PrevIndex, const int32 _NewIndex)
{
    checkf(_PrevIndex != _NewIndex, TEXT("index must be different"));

    if (mSortedBars.IsValidIndex(_PrevIndex))
    {
        mSortedBars[_PrevIndex].GetInterfaceMutable().OnUnselect(_World);
    }

    checkf(mSortedBars.IsValidIndex(_NewIndex), TEXT("index must be valid"));
    const FKLDebugFrameworkBottomBarSortedData& SelectedBottomBar = mSortedBars[_NewIndex];
    SelectedBottomBar.GetInterfaceMutable().OnSelect(_World);

    const FKLDebugContextGetterInput Input{ _World, _World.GetNetMode() };
    return SelectedBottomBar.GetInterface().GetContext(Input);
}

const IKLDebugBottomBarInterface* FKLDebugFrameworkBottomBarManager::TryGetCurrentInterface(const int32 _Index) const
{
    if (mSortedBars.IsValidIndex(_Index))
    {
        return &mSortedBars[_Index].GetInterface();
    }

    return nullptr;
}

IKLDebugBottomBarInterface* FKLDebugFrameworkBottomBarManager::TryGetCurrentInterfaceMutable(const int32 _Index)
{
    if (mSortedBars.IsValidIndex(_Index))
    {
        return &mSortedBars[_Index].GetInterfaceMutable();
    }

    return nullptr;
}

void FKLDebugFrameworkBottomBarManager::DrawBottomBar(const int32 _CurrentBottomBar, const UWorld& _World, IKLDebugContextInterface* _Context) const
{
    if (!mSortedBars.IsValidIndex(_CurrentBottomBar))
    {
        return;
    }

    const FKLDebugFrameworkBottomBarSortedData& SelectedBottomBar = mSortedBars[_CurrentBottomBar];
    const FKLDebugBottomBarDrawInput Input{ _World, _Context };
    SelectedBottomBar.GetInterface().Draw(Input);
}
