#include "ImGui/Visualizer/Tree/KLDebugTestSuiteImGuiVisualizerTreeTest.h"

#include "ImGui/Features/Helpers/KLDebugTestSuiteCreateTestFeatures.h"

// imgui module
#include "ImGui/Private/Feature/Container/KLDebugImGuiFeatureData.h"
#include "ImGui/Private/Feature/Container/KLDebugImGuiFeaturesIterator.h"
#include "ImGui/Private/Helpers/KLDebugImGuiTreeBuilderHelpers.h"
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"
#include "ImGui/Public/Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerNodeData.h"
#include "ImGui/Public/Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"
#include "ImGui/Public/Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeNode.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "AITestsCommon.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::InstantTest()
{
    uint32 FullSize = 0;
    FullSize += sizeof(FKLDebugTestCoverFeature);
    FullSize += sizeof(FKLDebugTestCoverChildFeature);
    FullSize += sizeof(FKLDebugTestCoverSecondChildFeature);
    FullSize += sizeof(FKLDebugTestCoverThirdChildFeature);
    FullSize += sizeof(FKLDebugTestCoverThirdChildChildFeature);
    FullSize += sizeof(FKLDebugTestCoverChildChildFeature);
    FullSize += sizeof(FKLDebugTestCoverChildChildChildFeature);
    FullSize += sizeof(FKLDebugTestPerceptionFeature);

    TArray<FKLDebugImGuiFeatureData> FeatureData;
    FeatureData.Reserve(8);

    TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue> FeaturePool;
    FeaturePool.AddZeroed(FullSize);

    TArray<FString> ImGuiPathTokens;
    ImGuiPathTokens.Reserve(20);

    KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = 0;
    for (int32 i = 0; i < 8; ++i)
    {
        uint32                             Size           = 0;
        IKLDebugImGuiFeatureInterfaceBase* DebugWindow    = nullptr;
        FKLDebugImGuiFeatureData&          NewFeatureData = FeatureData.Emplace_GetRef(FeatureOffset);
        switch (i)
        {
            case 0:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestCoverFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverFeature);
                break;
            case 1:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestCoverChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverChildFeature);
                break;
            case 2:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestCoverSecondChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverSecondChildFeature);
                break;
            case 3:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestCoverThirdChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverThirdChildFeature);
                break;
            case 4:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestCoverThirdChildChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverThirdChildChildFeature);
                break;
            case 5:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestCoverChildChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverChildChildFeature);
                break;
            case 6:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestCoverChildChildChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverChildChildChildFeature);
                break;
            case 7:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&FeaturePool[FeatureOffset])) FKLDebugTestPerceptionFeature());
                FeatureOffset += sizeof(FKLDebugTestPerceptionFeature);
                break;
            default:
                checkNoEntry();
                break;
        }

        NewFeatureData.Init(*DebugWindow, ImGuiPathTokens);
    }

    FKLDebugImGuiFeaturesIterator      Iterator(FeatureData, FeaturePool);
    FKLDebugImGuiFeatureVisualizerTree TreeVisualizer;
    TreeVisualizer.CreateTree(Iterator);

    TreeOrderArray ExpectedTreeOrder;
    AITEST_TRUE("Parsing of the tree found a bug", TestPreorderTraversal(TreeVisualizer, ExpectedTreeOrder));
    AITEST_TRUE("Parsing of the tree with closed nodes found a bug", TestPreorderTraversalWithClosedNodes(TreeVisualizer, ExpectedTreeOrder));
    AITEST_TRUE("Parsing of the tree as imgui found a bug", TestImGuiPreorderTraversal(TreeVisualizer, ExpectedTreeOrder));
    return true;
}

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::TestPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements)
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
        const uint16                                  DataIndex = _TreeNode.GetNodeDataIndex();
        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData  = TreeNodesData[DataIndex];

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

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::TestPreorderTraversalWithClosedNodes(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements)
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
        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData = TreeNodesData[_TreeNode.GetNodeDataIndex()];
        const FName                                   NodeName(NodeData.GetImGuiNodeString());
        return Success && ClosedNodes.Find(NodeName) == INDEX_NONE;
    };

    auto EvaluateNodeLambda = [&TreeNodesData, &Index, &_ExpetectedElements, &Success](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        const uint16                                  DataIndex = _TreeNode.GetNodeDataIndex();
        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData  = TreeNodesData[DataIndex];

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

bool FKLDebugTestSuiteImGuiVisualizerTreeTest::TestImGuiPreorderTraversal(const FKLDebugImGuiFeatureVisualizerTree& _TreeVisualizer, TreeOrderArray& _ExpetectedElements)
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
        const uint16                                  DataIndex = _TreeNode.GetNodeDataIndex();
        const FKLDebugImGuiFeatureVisualizerNodeData& NodeData  = TreeNodesData[DataIndex];

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