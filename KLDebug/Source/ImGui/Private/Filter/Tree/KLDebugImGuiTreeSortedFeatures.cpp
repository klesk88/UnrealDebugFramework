#include "Filter/Tree/KLDebugImGuiTreeSortedFeatures.h"

FKLDebugImGuiTreeSortedFeatures::FKLDebugImGuiTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _Offset, const FName& _FeaturePath, const TArray<FName>& _Filters, const IKLDebugImGuiFeatureInterface_SelectableObject& _Feature)
    : FKLDebugImGuiTreeBuilderData(_Offset, _FeaturePath)
    , mFeature(&_Feature)
    , mFilters(_Filters)
{
}
