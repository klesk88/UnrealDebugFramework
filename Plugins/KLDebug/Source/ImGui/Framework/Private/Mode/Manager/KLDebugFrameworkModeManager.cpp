// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Mode/Manager/KLDebugFrameworkModeManager.h"
#include "Config/KLDebugImGuiConfig.h"

// modules
#include "User/Framework/Internal/Mode/StaticMemory/KLDebugModeManager.h"
#include "User/Framework/Internal/Mode/StaticMemory/KLDebugModeManagerEntryBase.h"
#include "User/Framework/Public/Mode/KLDebugModeInterface.h"
#include "User/Framework/Public/Mode/KLDebugModeInterfaceAllInput.h"

// engine
#include "Engine/World.h"

void FKLDebugFrameworkModeManager::Initialize()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugFrameworkModeManager_Initialize);

    InitInternal<FKLDebugModeManager, FKLDebugModeManagerEntryBase>();

    mSortedModes.Reserve(mIDToData.Num());
    for (TMap<FName, uint32>::TConstIterator Iter(mIDToData.CreateConstIterator()); Iter; ++Iter)
    {
        const FName& Key = Iter.Key();
        IKLDebugModeInterface& Interface = GetDataMutable(Iter.Value());
        mSortedModes.Emplace(Interface.GetFriendlyName(), Interface);
    }

    mSortedModes.Sort([](const FKLDebugFrameworkModeSortedData& _Left, const FKLDebugFrameworkModeSortedData& _Right) -> bool {
        return _Left.GetName() < _Right.GetName();
    });
}

void FKLDebugFrameworkModeManager::Shutdown()
{
}

TUniquePtr<IKLDebugContextInterface> FKLDebugFrameworkModeManager::UpdateBottomBarIfNeeded(const UWorld& _World, const int32 _PrevIndex, const int32 _NewIndex)
{
    checkf(_PrevIndex != _NewIndex, TEXT("index must be different"));

    if (mSortedModes.IsValidIndex(_PrevIndex))
    {
        mSortedModes[_PrevIndex].GetInterfaceMutable().OnUnselect(_World);
    }

    checkf(mSortedModes.IsValidIndex(_NewIndex), TEXT("index must be valid"));
    const FKLDebugFrameworkModeSortedData& SelectedBottomBar = mSortedModes[_NewIndex];
    SelectedBottomBar.GetInterfaceMutable().OnSelect(_World);

    const FKLDebugContextGetterInput Input{ _World, _World.GetNetMode() };
    return SelectedBottomBar.GetInterface().GetContext(Input);
}

bool FKLDebugFrameworkModeManager::RequireCanvasDraw(const int32 _CurrentIndex) const
{
    if (!mSortedModes.IsValidIndex(_CurrentIndex))
    {
        return false;
    }

    const FKLDebugFrameworkModeSortedData& SelectedMode = mSortedModes[_CurrentIndex];
    return SelectedMode.GetInterface().RequiresDrawCanvas();
}

void FKLDebugFrameworkModeManager::DrawImGui(const int32 _CurrentIndex, const UWorld& _World, IKLDebugContextInterface* _Context) const
{
    if (!mSortedModes.IsValidIndex(_CurrentIndex))
    {
        return;
    }

    const FKLDebugFrameworkModeSortedData& SelectedMode = mSortedModes[_CurrentIndex];
    const FKLDebugModeDrawImGuiInput Input{ _World, _Context };
    SelectedMode.GetInterfaceMutable().DrawImGui(Input);
}

void FKLDebugFrameworkModeManager::DrawCanvas(const int32 _CurrentIndex, UWorld& _World, UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context) const
{
    if (!mSortedModes.IsValidIndex(_CurrentIndex))
    {
        return;
    }

    const FKLDebugFrameworkModeSortedData& SelectedMode = mSortedModes[_CurrentIndex];
    if (SelectedMode.GetInterface().RequiresDrawCanvas())
    {
        FKLDebugModeDrawCanvasInput Input{ _World, _Canvas, _Font, _Context };

        const UKLDebugImGuiConfig& Config = UKLDebugImGuiConfig::Get();
        const FKLDebugImGuiConfig_Canvas& CanvasConfig = Config.GetCanvasConfig();

        Input.CursorX = Input.DefaultX = CanvasConfig.GetBorderOffsetX();
        Input.CursorY = Input.DefaultY = CanvasConfig.GetBorderOffsetY();
        Input.World = &_World;

        SelectedMode.GetInterface().DrawCanvas(Input);
    }
}