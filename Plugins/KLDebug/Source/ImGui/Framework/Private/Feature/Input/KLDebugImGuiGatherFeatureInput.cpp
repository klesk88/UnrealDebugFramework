// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Input/KLDebugImGuiGatherFeatureInput.h"

FKLDebugImGuiGatherFeatureInput::FKLDebugImGuiGatherFeatureInput(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const EImGuiInterfaceType _ContainerType, const UObject* _Object, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
    : mFeaturesContainerManager(_ContainerManager)
    , mObject(_Object)
    , mContainerType(_ContainerType)
    , mFeatureIndex(_FeatureIndex)
{
}
