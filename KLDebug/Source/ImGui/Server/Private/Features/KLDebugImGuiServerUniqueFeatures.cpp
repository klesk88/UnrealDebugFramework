#include "Features/KLDebugImGuiServerUniqueFeatures.h"

void FKLDebugImGuiServerUniqueFeatures::AddFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _Feature)
{
    if (mFeatures.Find(_Feature) != INDEX_NONE)
    {
        ensureMsgf(false, TEXT("feature already enabled"));
        return;
    }

    mFeatures.Emplace(_Feature);
}

void FKLDebugImGuiServerUniqueFeatures::RemoveFeature(KL::Debug::ImGui::Features::Types::FeatureIndex _Feature)
{
    const int32 Index = mFeatures.IndexOfByKey(_Feature);
    if (Index == INDEX_NONE)
    {
        ensureMsgf(false, TEXT("feature not present"));
        return;
    }

    mFeatures.RemoveAtSwap(Index, 1, false);
}
