#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"

FKLDebugImGuiFeatureVisualizerEntry::FKLDebugImGuiFeatureVisualizerEntry(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeaturesIndex, const KL::Debug::ImGui::Features::VisualizerTree::NodeDataID _NodeDataID)
    : mFeatureIndex(_FeaturesIndex)
    , mNodeDataID(_NodeDataID)
{
}
