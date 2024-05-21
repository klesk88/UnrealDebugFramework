// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "AITestsCommon.h"

class FKLDebugImGuiFeatureVisualizerTree;

class FKLDebugTestSuiteImGuiVisualizerTreeTest final : public FAITestBase
{
private:
    using TreeOrderArray = TArray<FName, TInlineAllocator<16>>;

public:
    [[nodiscard]] bool InstantTest() final;

private:
    [[nodiscard]] bool TestPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
    [[nodiscard]] bool TestPreorderTraversalWithClosedNodes(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
    [[nodiscard]] bool TestImGuiPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
};