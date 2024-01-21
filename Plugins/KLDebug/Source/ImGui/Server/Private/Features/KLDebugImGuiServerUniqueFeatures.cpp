// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/KLDebugImGuiServerUniqueFeatures.h"

void FKLDebugImGuiServerUniqueFeatures::AddFeature(const FKLDebugContextGetterInput& _Input, const IKLDebugFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeature, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeature)
{
    if (mFeatures.FindByKey(_ServerFeature) != nullptr)
    {
        ensureMsgf(false, TEXT("feature already enabled"));
        return;
    }

    mFeatures.Emplace(_Input, _FeatureInterface, _ClientFeature, _ServerFeature);
}

void FKLDebugImGuiServerUniqueFeatures::RemoveFeature(KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeature)
{
    const int32 Index = mFeatures.IndexOfByKey(_ServerFeature);
    if (Index == INDEX_NONE)
    {
        ensureMsgf(false, TEXT("feature not present"));
        return;
    }

    mFeatures.RemoveAtSwap(Index, 1, false);
}
