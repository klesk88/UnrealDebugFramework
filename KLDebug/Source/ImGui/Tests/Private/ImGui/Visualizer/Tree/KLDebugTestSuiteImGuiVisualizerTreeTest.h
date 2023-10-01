#pragma once

// engine
#include "AITestsCommon.h"

class FKLDebugImGuiFeatureVisualizerTree;

class FKLDebugTestSuiteImGuiVisualizerTreeTest final : public FAITestBase
{
private:
    using TreeOrderArray = TArray<FName, TInlineAllocator<16>>;

public:
    UE_NODISCARD bool InstantTest() final;

private:
    UE_NODISCARD bool TestPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
    UE_NODISCARD bool TestPreorderTraversalWithClosedNodes(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
    UE_NODISCARD bool TestImGuiPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
};