// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"

#include "Context/KLDebugContextInterface.h"

FKLDebugImGuiFeatureVisualizerEntry::FKLDebugImGuiFeatureVisualizerEntry(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeaturesIndex, const KL::Debug::ImGui::Features::VisualizerTree::NodeDataID _NodeDataID, TUniquePtr<IKLDebugContextInterface>&& _FeatureContext)
    : mFeatureContext(MoveTemp(_FeatureContext))
    , mFeatureIndex(_FeaturesIndex)
    , mNodeDataID(_NodeDataID)
{
}

FKLDebugImGuiFeatureVisualizerEntry::~FKLDebugImGuiFeatureVisualizerEntry()
{
    mFeatureContext.Reset();
}

IKLDebugContextInterface* FKLDebugImGuiFeatureVisualizerEntry::TryGetFeatureContextMutable() const
{
    return mFeatureContext.Get();
}
