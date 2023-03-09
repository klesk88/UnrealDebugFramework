#include "Feature/Container/KLDebugImGuiFeaturesIterator.h"

FKLDebugImGuiFeaturesIterator::FKLDebugImGuiFeaturesIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& _FeaturesPool)
    : mFeatureData(_FeatureData)
    , mFeaturesPool(_FeaturesPool)
{
}
