#include "TreeBuilder/KLDebugImGuiTreeBuilderData.h"

FKLDebugImGuiTreeBuilderData::FKLDebugImGuiTreeBuilderData(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex, const FName& _FeatureImGuiPath)
    : mTreePath(_FeatureImGuiPath)
    , mFeatureDataIndex(_FeatureDataIndex)
{
}