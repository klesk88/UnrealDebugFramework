// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "TreeBuilder/KLDebugImGuiTreeBuilderData.h"

FKLDebugImGuiTreeBuilderData::FKLDebugImGuiTreeBuilderData(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex, const FName& _FeatureImGuiPath)
    : mTreePath(_FeatureImGuiPath)
    , mFeatureDataIndex(_FeatureDataIndex)
{
}