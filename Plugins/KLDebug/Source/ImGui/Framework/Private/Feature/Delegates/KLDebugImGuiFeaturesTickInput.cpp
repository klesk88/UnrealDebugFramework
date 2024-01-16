// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Delegates/KLDebugImGuiFeaturesTickInput.h"

FKLDebugImGuiFeaturesTickInput::FKLDebugImGuiFeaturesTickInput(const UWorld& _World, const int32 _CurrentModeIndex, const int32 _BottomBarIndex, FKLDebugImGuiFeatureVisualizer_Unique& _UniqueFeatures, TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& _ObjectFeatures, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _Flags, IKLDebugModeInterface* _CurrentMode, IKLDebugContextInterface* _CurrentModeContext, IKLDebugBottomBarInterface* _CurrentBottomBar, IKLDebugContextInterface* _BottomBarContext)
    : mWorld(_World)
    , mUniqueFeatures(_UniqueFeatures)
    , mObjectFeatures(_ObjectFeatures)
    , mContainerManager(_ContainerManager)
    , mFlags(_Flags)
    , mCurrentModeIndex(_CurrentModeIndex)
    , mCurrentMode(_CurrentMode)
    , mCurrentModeContext(_CurrentModeContext)
    , mBottomBarIndex(_BottomBarIndex)
    , mCurrentBottomBar(_CurrentBottomBar)
    , mBottomBarContext(_BottomBarContext)
{
}
