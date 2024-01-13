// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Delegates/KLDebugImGuiFeaturesTickInput.h"

FKLDebugImGuiFeaturesTickInput::FKLDebugImGuiFeaturesTickInput(const UWorld& _World, FKLDebugImGuiFeatureVisualizer_Unique& _UniqueFeatures, TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& _ObjectFeatures, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _Flags)
    : mWorld(_World)
    , mUniqueFeatures(_UniqueFeatures)
    , mObjectFeatures(_ObjectFeatures)
    , mContainerManager(_ContainerManager)
    , mFlags(_Flags)
{
}
