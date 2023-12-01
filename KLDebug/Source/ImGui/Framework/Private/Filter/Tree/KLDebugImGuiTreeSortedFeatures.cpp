// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Filter/Tree/KLDebugImGuiTreeSortedFeatures.h"

FKLDebugImGuiTreeSortedFeatures::FKLDebugImGuiTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _Offset, const FName& _FeaturePath, const TArray<FName>& _Filters, const IKLDebugImGuiFeatureInterface_Selectable& _Feature)
    : FKLDebugImGuiTreeBuilderData(_Offset, _FeaturePath)
    , mFeature(&_Feature)
    , mFilters(_Filters)
{
}
