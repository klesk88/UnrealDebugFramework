#pragma once

// engine
#include "AITestsCommon.h"

class FKLDebugImGuiFilterTree;

class FKLDebugTestSuiteImGuiFilterTreeTest final : public FAITestBase
{
private:
    using TreeOrderArray = TArray<FName, TInlineAllocator<16>>;

public:
    UE_NODISCARD bool InstantTest() final;

private:
    void GenerateTreeData(FKLDebugImGuiFilterTree& _FilterTree) const;

    UE_NODISCARD bool TestFilterOrder() const;
    UE_NODISCARD bool TestPreorderTraversal(const FKLDebugImGuiFilterTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;
    UE_NODISCARD bool TestFeaturesIndexesSet(const FKLDebugImGuiFilterTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const;

    void GatherTreeNodesInOrder(TreeOrderArray& _ExpetectedElements) const;
};