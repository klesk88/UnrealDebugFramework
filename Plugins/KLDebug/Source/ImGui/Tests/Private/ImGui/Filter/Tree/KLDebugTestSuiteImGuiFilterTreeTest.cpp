// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGui/Filter/Tree/KLDebugTestSuiteImGuiFilterTreeTest.h"

#include "ImGui/Features/Helpers/KLDebugTestSuiteCreateFeaturesHelpers.h"
#include "ImGui/Features/Helpers/KLDebugTestSuiteTestFeatures.h"
#include "ImGui/Filter/Helpers/KLDebugTestSuiteTestFilters.h"
#include "ImGui/Filter/Helpers/KLDebugTestSuiteTestFiltersHelpers.h"

// modules
#include "ImGui/Framework/Private/Filter/Tree/KLDebugImGuiFilterTree.h"
#include "ImGui/Framework/Private/Filter/Tree/KLDebugImGuiFilterTreeNode.h"
#include "ImGui/Framework/Private/TreeBuilder/KLDebugImGuiTreeBuilderHelpers.h"
#include "ImGui/Framework/Public/Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureData.h"
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"
#include "User/Framework/Public/Filter/Interface/KLDebugFilterInterface.h"

// engine
#include "AITestsCommon.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"

bool FKLDebugTestSuiteImGuiFilterTreeTest::InstantTest()
{
    AITEST_TRUE("Fitlers are not in the correct order when querying for them", TestFilterOrder());

    TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue> FeaturePool;
    TArray<FKLDebugImGuiFeatureData> FeatureData;

    KL::Debug::TestSuite::Feature::Helpers::GetFeatures(FeaturePool, FeatureData);

    FKLDebugImGuiFeaturesIterator Iterator(FeatureData, FeaturePool);
    FKLDebugImGuiFilterTree FilterTree;

    GenerateTreeData(FilterTree);
    FilterTree.TestGenerateTree(FeatureData.Num(), Iterator);

    static constexpr int32 TreeNodesCount = 10;
    AITEST_TRUE("Tree nodes count should be 10", FilterTree.TestGetTreeNodes().Num() == TreeNodesCount);

    TreeOrderArray ExpectedTreeOrder;
    AITEST_TRUE("Parsing of the tree found a bug", TestPreorderTraversal(FilterTree, ExpectedTreeOrder));
    AITEST_TRUE("Parsing of the features found a bug", TestFeaturesIndexesSet(FilterTree, ExpectedTreeOrder));

    return true;
}

void FKLDebugTestSuiteImGuiFilterTreeTest::GenerateTreeData(FKLDebugImGuiFilterTree& _FilterTree) const
{
    TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue> FilterPool;
    TArray<KL::Debug::ImGui::Features::Types::FilterIndex> FilterOffsets;

    KL::Debug::TestSuite::Filter::Helpers::GetFilters(FilterPool, FilterOffsets);

    _FilterTree.TestInitWithExternalData(MoveTemp(FilterPool), MoveTemp(FilterOffsets));
}

bool FKLDebugTestSuiteImGuiFilterTreeTest::TestFilterOrder() const
{
    static constexpr int32 Count = 4;

    FKLDebugTestCoverChildFeature TestFeature;
    TArray<FName> Filters;
    Filters.Reserve(Count);
    TestFeature.GetFilterPath(Filters);
    bool Success = true;

    for (int32 i = 0; i < Count; ++i)
    {
        switch (i)
        {
        case 0:
            Success &= Filters[i] == KLDebugImGuiTestFilterOne::StaticGetFilterID();
            break;
        case 1:
            Success &= Filters[i] == KLDebugImGuiTestFilterTwo::StaticGetFilterID();
            break;
        case 2:
            Success &= Filters[i] == KLDebugImGuiTestFilterThree::StaticGetFilterID();
            break;
        case 3:
            Success &= Filters[i] == KLDebugImGuiTestFilterFour::StaticGetFilterID();
            break;
        }
    }

    return Success;
}

bool FKLDebugTestSuiteImGuiFilterTreeTest::TestPreorderTraversal(const FKLDebugImGuiFilterTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const
{
    const TArray<FKLDebugImGuiFilterTreeNode>& TreeNodes = _TreeVisualizer.TestGetTreeNodes();
    const TArray<FKLDebugImGuiFilterTreeNodeData>& TreeNodesData = _TreeVisualizer.TestGetTreeNodesData();
    const TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& FilterPool = _TreeVisualizer.TestGetFilterPool();

    GatherTreeNodesInOrder(_ExpetectedElements);

    int32 Index = 0;
    bool Success = true;

    auto KeepTraversingTreeLambda = [&Success](const FKLDebugImGuiFilterTreeNode& _TreeNode) -> bool {
        return Success;
    };

    auto EvaluateNodeLambda = [&TreeNodesData, &Index, &_ExpetectedElements, &FilterPool, &Success](const FKLDebugImGuiFilterTreeNode& _TreeNode) -> void {
        const IKLDebugFilterInterface* FilterInterface = reinterpret_cast<const IKLDebugFilterInterface*>(&FilterPool[_TreeNode.GetFilterIndex()]);

        const FName& ExpectedTreeNodeName = _ExpetectedElements[Index++];
        if (FilterInterface->GetFilterID() != ExpectedTreeNodeName)
        {
            Success = false;
        }
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::PreoderTraversal(TreeNodes, KeepTraversingTreeLambda, EvaluateNodeLambda);
    return Success;
}

bool FKLDebugTestSuiteImGuiFilterTreeTest::TestFeaturesIndexesSet(const FKLDebugImGuiFilterTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const
{
    /*
     * TestNine
     * TestOne.TestTwo
     * TestOne.TestTwo.TestFour.TestThree
     * TestOne.TestTwo.TestTen
     * TestOne.TestTwo.TestThree.TestFour
     * TestSeven.TestHeight
     */

    const TArray<FKLDebugImGuiFilterTreeNode>& TreeNodes = _TreeVisualizer.TestGetTreeNodes();
    const TArray<FKLDebugImGuiFilterTreeNodeData>& TreeNodesData = _TreeVisualizer.TestGetTreeNodesData();
    const TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& FilterPool = _TreeVisualizer.TestGetFilterPool();

    _ExpetectedElements.Reset();
    _ExpetectedElements.Emplace(TEXT("TestNine"));
    _ExpetectedElements.Emplace(TEXT("TestTwo"));
    _ExpetectedElements.Emplace(TEXT("TestThree"));
    _ExpetectedElements.Emplace(TEXT("TestTen"));
    _ExpetectedElements.Emplace(TEXT("TestFour"));
    _ExpetectedElements.Emplace(TEXT("TestHeight"));

    int32 Index = 0;
    bool Success = true;

    auto KeepTraversingTreeLambda = [&Success](const FKLDebugImGuiFilterTreeNode& _TreeNode) -> bool {
        return Success;
    };

    auto EvaluateNodeLambda = [&TreeNodesData, &Index, &_ExpetectedElements, &FilterPool, &Success](const FKLDebugImGuiFilterTreeNode& _TreeNode) -> void {
        const IKLDebugFilterInterface* FilterInterface = reinterpret_cast<const IKLDebugFilterInterface*>(&FilterPool[_TreeNode.GetFilterIndex()]);
        const TOptional<uint16> NodeDataIndex = _TreeNode.GetNodeDataIndex();
        if (!NodeDataIndex.IsSet())
        {
            return;
        }

        const FKLDebugImGuiFilterTreeNodeData& NodeData = TreeNodesData[NodeDataIndex.GetValue()];
        if (NodeData.GetStartDataOffset() == TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max() || NodeData.GetEndDataOffset() == TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max())
        {
            ensureMsgf(false, TEXT("both should be set"));
            Success = false;
            return;
        }

        const FName& ExpectedTreeNodeName = _ExpetectedElements[Index++];
        if (FilterInterface->GetFilterID() != ExpectedTreeNodeName)
        {
            Success = false;
        }
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::PreoderTraversal(TreeNodes, KeepTraversingTreeLambda, EvaluateNodeLambda);
    return Success && Index == _ExpetectedElements.Num();
}

void FKLDebugTestSuiteImGuiFilterTreeTest::GatherTreeNodesInOrder(TreeOrderArray& _ExpetectedElements) const
{
    /*
     *   ai.cover.1 -> TestOne.TestTwo
     *   ai.cover.1.a -> TestOne.TestTwo.TestThree.TestFour
     *   ai.cover.1.a.1 -> TestOne.TestTwo
     *   ai.cover.1.b -> TestOne.TestTwo.TestFour.TestThree
     *   ai.cover.2 -> TestOne.TestTwo.TestTen
     *   ai.cover.3.a -> TestNine
     *   ai.cover.3.b -> TestSeven.TestHeight
     *   ai.perception
     */

    /*
     * TestNine
     * TestOne.TestTwo
     * TestOne.TestTwo.TestFour.TestThree
     * TestOne.TestTwo.TestTen
     * TestOne.TestTwo.TestThree.TestFour
     * TestSeven.TestHeight
     */

    _ExpetectedElements.Reset();
    _ExpetectedElements.Emplace(TEXT("TestNine"));
    _ExpetectedElements.Emplace(TEXT("TestOne"));
    _ExpetectedElements.Emplace(TEXT("TestTwo"));
    _ExpetectedElements.Emplace(TEXT("TestFour"));
    _ExpetectedElements.Emplace(TEXT("TestThree"));
    _ExpetectedElements.Emplace(TEXT("TestTen"));
    _ExpetectedElements.Emplace(TEXT("TestThree"));
    _ExpetectedElements.Emplace(TEXT("TestFour"));
    _ExpetectedElements.Emplace(TEXT("TestSeven"));
    _ExpetectedElements.Emplace(TEXT("TestHeight"));
}

IMPLEMENT_AI_INSTANT_TEST(FKLDebugTestSuiteImGuiFilterTreeTest, "KLDebug.ImGui.Filter.Tree")