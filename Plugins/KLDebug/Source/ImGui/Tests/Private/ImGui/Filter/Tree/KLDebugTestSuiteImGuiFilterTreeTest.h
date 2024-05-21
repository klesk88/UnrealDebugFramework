// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "AITestsCommon.h"

class FKLDebugImGuiFilterTree;

class FKLDebugTestSuiteImGuiFilterTreeTest final : public FAITestBase
{
private:
    using TreeOrderArray = TArray<FName, TInlineAllocator<16>>;

public:
    [[nodiscard]] bool InstantTest() final;

private:
    void GenerateTreeData(FKLDebugImGuiFilterTree& _FilterTree) const;

    [[nodiscard]] bool TestFilterOrder() const;
    [[nodiscard]] bool TestPreorderTraversal(const FKLDebugImGuiFilterTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
    [[nodiscard]] bool TestFeaturesIndexesSet(const FKLDebugImGuiFilterTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;

    void GatherTreeNodesInOrder(TreeOrderArray& _ExpetectedElements) const;
};