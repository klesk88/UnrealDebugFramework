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

void FKLDebugFrameworkBottomBarManager::UpdateBottomBarIfNeeded(const UWorld& _World, const int32 _Index)
{
    if (_Index == mCurrentIndex)
    {
        return;
    }

    checkf(mSortedBars.IsValidIndex(_Index), TEXT("index must be valid"));
    mCurrentIndex = _Index;
    const FKLDebugFrameworkBottomBarSortedData& SelectedBottomBar = mSortedBars[_Index];
    const FKLDebugContextGetterInput Input{ _World, _World.GetNetMode() };
    mCurrentContext = SelectedBottomBar.GetInterface().GetContext(Input);
}

void FKLDebugFrameworkBottomBarManager::DrawBottomBar(const UWorld& _World) const
{
    if (!mSortedBars.IsValidIndex(mCurrentIndex))
    {
        return;
    }

    const FKLDebugFrameworkBottomBarSortedData& SelectedBottomBar = mSortedBars[mCurrentIndex];
    const FKLDebugBottomBarDrawInput Input{ _World, mCurrentContext.Get() };
    SelectedBottomBar.GetInterface().Draw(Input);
}
