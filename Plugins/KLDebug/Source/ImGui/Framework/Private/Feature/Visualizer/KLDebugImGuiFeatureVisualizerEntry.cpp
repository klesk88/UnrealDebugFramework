// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

FKLDebugImGuiFeatureVisualizerEntry::FKLDebugImGuiFeatureVisualizerEntry(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeaturesIndex, const KL::Debug::ImGui::Features::VisualizerTree::NodeDataID _NodeDataID, TUniquePtr<FKLDebugImGuiFeatureContext_Base>&& _FeatureContext)
    : mFeatureContext(MoveTemp(_FeatureContext))
    , mFeatureIndex(_FeaturesIndex)
    , mNodeDataID(_NodeDataID)
{
}

FKLDebugImGuiFeatureVisualizerEntry::~FKLDebugImGuiFeatureVisualizerEntry()
{
    mFeatureContext.Reset();
}

FKLDebugImGuiFeatureContext_Base* FKLDebugImGuiFeatureVisualizerEntry::TryGetFeatureContextMutable() const
{
    return mFeatureContext.Get();
}
