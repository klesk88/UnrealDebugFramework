#include "ImGui/Visualizer/Tree/KLDebugTestSuiteImGuiVisualizerTreeTest.h"

#include "ImGui/Features/Helpers/KLDebugTestSuiteCreateFeaturesHelpers.h"

// imgui module
#include "ImGui/Private/Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "ImGui/Private/Feature/Container/KLDebugImGuiFeatureData.h"
#include "ImGui/Private/Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerNodeData.h"
#include "ImGui/Private/Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"
#include "ImGui/Private/Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeNode.h"
#include "ImGui/Private/TreeBuilder/KLDebugImGuiTreeBuilderHelpers.h"
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "AITestsCommon.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::InstantTest()
{
    TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue> FeaturePool;
    TArray<FKLDebugImGuiFeatureData>                            FeatureData;

    KL::Debug::TestSuite::Feature::Helpers::GetFeatures(FeaturePool, FeatureData);

    FKLDebugImGuiFeaturesConstIterator Iterator(FeatureData, FeaturePool);
    FKLDebugImGuiFeatureVisualizerTree TreeVisualizer;
    TreeVisualizer.CreateTree(Iterator);

    TreeOrderArray ExpectedTreeOrder;
    AITEST_TRUE("Parsing of the tree found a bug", TestPreorderTraversal(TreeVisualizer, ExpectedTreeOrder));
    AITEST_TRUE("Parsing of the tree with closed nodes found a bug", TestPreorderTraversalWithClosedNodes(TreeVisualizer, ExpectedTreeOrder));
    AITEST_TRUE("Parsing of the tree as imgui found a bug", TestImGuiPreorderTraversal(TreeVisualizer, ExpectedTreeOrder));
    return true;
}

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::TestPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const
{
    /*
     *   ai.cover.1
     *   ai.cover.1.a
     *   ai.cover.1.a.1
     *   ai.cover.1.b
     *   ai.cover.2
     *   ai.cover.3.a
     *   ai.cover.3.b
     *   ai.perception
     */

    const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& TreeNodes     = _TreeVisualizer.TestGetTreeNodes();
    const TArray<FKLDebugImGuiFeatureVisualizerNodeData>& TreeNodesData = _TreeVisualizer.TestGetTreeNodesData();

    static constexpr int32 TreeNodesCount = 11;
    AITEST_TRUE("Tree nodes count should be 11", TreeNodes.Num() == TreeNodesCount);
    AITEST_TRUE("Tree nodes data count should be 11", TreeNodesData.Num() == TreeNodesCount);

    _ExpetectedElements.Reset();
    _ExpetectedElements.Emplace(TEXT("ai"));
    _ExpetectedElements.Emplace(TEXT("cover"));
    _ExpetectedElements.Emplace(TEXT("1"));
    _ExpetectedElements.Emplace(TEXT("a"));
    _ExpetectedElements.Emplace(TEXT("1"));
    _ExpetectedElements.Emplace(TEXT("b"));
    _ExpetectedElements.Emplace(TEXT("2"));
    _ExpetectedElements.Emplace(TEXT("3"));
    _ExpetectedElements.Emplace(TEXT("a"));
    _ExpetectedElements.Emplace(TEXT("b"));
    _ExpetectedElements.Emplace(TEXT("perception"));

    int32 Index   = 0;
    bool  Success = true;

    auto KeepTraversingTreeLambda = [&Success](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> bool {
        return Success;
    };

    auto EvaluateNodeLambda = [&TreeNodesData, &Index, &_ExpetectedElements, &Success](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        const TOptional<uint16> DataIndex = _TreeNode.GetNodeDataIndex();
        if (!DataIndex.IsSet())
        {
            ensureMsgf(false, TEXT("we expect to have always node data"));
            Success = false;
            return;
        }

        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData = TreeNodesData[DataIndex.GetValue()];

        const FName  NodeName(NodeData.GetImGuiNodeString());
        const FName& ExpectedTreeNodeName = _ExpetectedElements[Index++];
        if (NodeName != ExpectedTreeNodeName)
        {
            Success = false;
        }
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::PreoderTraversal(TreeNodes, KeepTraversingTreeLambda, EvaluateNodeLambda);
    return Success;
}

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::TestPreorderTraversalWithClosedNodes(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const
{
    /*
     *   ai.cover.1
     *   ai.cover.1.a
     *   ai.cover.1.a.1
     *   ai.cover.1.b
     *   ai.cover.2
     *   ai.cover.3.a
     *   ai.cover.3.b
     *   ai.perception
     */

    const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& TreeNodes     = _TreeVisualizer.TestGetTreeNodes();
    const TArray<FKLDebugImGuiFeatureVisualizerNodeData>& TreeNodesData = _TreeVisualizer.TestGetTreeNodesData();

    TArray<FName, TInlineAllocator<2>> ClosedNodes;
    ClosedNodes.Emplace("1");
    ClosedNodes.Emplace("perception");

    _ExpetectedElements.Reset();
    _ExpetectedElements.Emplace(TEXT("ai"));
    _ExpetectedElements.Emplace(TEXT("cover"));
    _ExpetectedElements.Emplace(TEXT("2"));
    _ExpetectedElements.Emplace(TEXT("3"));
    _ExpetectedElements.Emplace(TEXT("a"));
    _ExpetectedElements.Emplace(TEXT("b"));

    int32 Index   = 0;
    bool  Success = true;

    auto KeepTraversingTreeLambda = [&Success, &ClosedNodes, &TreeNodesData](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> bool {
        const TOptional<uint16> NodeDataIndex = _TreeNode.GetNodeDataIndex();
        if (!NodeDataIndex.IsSet())
        {
            ensureMsgf(false, TEXT("we expect to have always node data"));
            Success = false;
            return true;
        }

        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData = TreeNodesData[NodeDataIndex.GetValue()];
        const FName                                   NodeName(NodeData.GetImGuiNodeString());
        return Success && ClosedNodes.Find(NodeName) == INDEX_NONE;
    };

    auto EvaluateNodeLambda = [&TreeNodesData, &Index, &_ExpetectedElements, &Success](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        const TOptional<uint16> NodeDataIndex = _TreeNode.GetNodeDataIndex();
        if (!NodeDataIndex.IsSet())
        {
            ensureMsgf(false, TEXT("we expect to have always node data"));
            Success = false;
            return;
        }

        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData = TreeNodesData[NodeDataIndex.GetValue()];

        const FName  NodeName(NodeData.GetImGuiNodeString());
        const FName& ExpectedTreeNodeName = _ExpetectedElements[Index++];
        if (NodeName != ExpectedTreeNodeName)
        {
            Success = false;
        }
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::PreoderTraversal(TreeNodes, KeepTraversingTreeLambda, EvaluateNodeLambda);
    return Success;
}

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::TestImGuiPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements) const
{
    /*
     *   ai.cover.1
     *   ai.cover.1.a
     *   ai.cover.1.a.1
     *   ai.cover.1.b
     *   ai.cover.2
     *   ai.cover.3.a
     *   ai.cover.3.b
     *   ai.perception
     */

    _ExpetectedElements.Reset();
    _ExpetectedElements.Emplace(TEXT("ai"));
    _ExpetectedElements.Emplace(TEXT("cover"));
    _ExpetectedElements.Emplace(TEXT("1"));
    _ExpetectedElements.Emplace(TEXT("a"));
    _ExpetectedElements.Emplace(TEXT("1"));
    _ExpetectedElements.Emplace(TEXT("a"));  // closing a subtree
    _ExpetectedElements.Emplace(TEXT("b"));
    _ExpetectedElements.Emplace(TEXT("1"));  // closing 1 subtree
    _ExpetectedElements.Emplace(TEXT("2"));
    _ExpetectedElements.Emplace(TEXT("3"));
    _ExpetectedElements.Emplace(TEXT("a"));
    _ExpetectedElements.Emplace(TEXT("b"));
    _ExpetectedElements.Emplace(TEXT("3"));      // closing 3 subtree
    _ExpetectedElements.Emplace(TEXT("cover"));  // closing cover subtree
    _ExpetectedElements.Emplace(TEXT("perception"));
    _ExpetectedElements.Emplace(TEXT("ai"));  // closing ai subtree

    const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& TreeNodes     = _TreeVisualizer.TestGetTreeNodes();
    const TArray<FKLDebugImGuiFeatureVisualizerNodeData>& TreeNodesData = _TreeVisualizer.TestGetTreeNodesData();
    int32                                                 Index         = 0;
    bool                                                  Success       = true;

    auto KeepTraversingTreeLambda = [&Success](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> bool {
        return Success;
    };

    auto EvaluateNodeLambda = [&TreeNodesData, &Index, &_ExpetectedElements, &Success](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        const TOptional<uint16> NodeDataIndex = _TreeNode.GetNodeDataIndex();
        if (!NodeDataIndex.IsSet())
        {
            ensureMsgf(false, TEXT("we expect to have always node data"));
            Success = false;
            return;
        }

        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData = TreeNodesData[NodeDataIndex.GetValue()];

        const FName  NodeName(NodeData.GetImGuiNodeString());
        const FName& ExpectedTreeNodeName = _ExpetectedElements[Index++];
        if (NodeName != ExpectedTreeNodeName)
        {
            Success = false;
        }
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::PreoderTraversalImGui(TreeNodes, KeepTraversingTreeLambda, EvaluateNodeLambda, EvaluateNodeLambda);
    return Success;
}

IMPLEMENT_AI_INSTANT_TEST(FKLDebugTestSuiteImGuiVisualizerTreeTest, "KLDebug.ImGui.Feature.TreeVisualizer")