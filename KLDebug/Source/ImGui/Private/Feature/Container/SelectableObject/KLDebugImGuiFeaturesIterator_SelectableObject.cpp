#include "Feature/Container/SelectableObject/KLDebugImGuiFeaturesIterator_SelectableObject.h"

FKLDebugImGuiFeaturesIterator_SelectableObject::FKLDebugImGuiFeaturesIterator_SelectableObject(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& _FeaturesPool)
    : mFeatureData(_FeatureData)
    , mFeaturesIndexes(_FeaturesIndexes)
    , mFeaturesPool(_FeaturesPool)
{
}
