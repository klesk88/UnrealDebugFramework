#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"

#include "Feature/Container/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerStackData.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeHelpers.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiVisualizerTreeSortedFeatures.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"
#include "Math/UnrealMathUtility.h"

void FKLDebugImGuiFeatureVisualizerTree::CreateTree(FKLDebugImGuiFeaturesIterator& _Iterator)
{
    if (_Iterator.GetFeaturesCount() == 0)
    {
        return;
    }

    TArray<FKLDebugImGuiVisualizerTreeSortedFeatures> SortedFeatures;
    GatherAndSortFeatures(_Iterator, SortedFeatures);
    GenerateTree(SortedFeatures);
}

void FKLDebugImGuiFeatureVisualizerTree::DrawImGuiTree()
{
    static constexpr ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    static constexpr ImGuiTreeNodeFlags LeafFlags = BaseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Selected;

    auto KeepTraversingTreeLambda = [this](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> bool {
        FKLDebugImGuiFeatureVisualizerNodeData& NodeData = mNodesData[_TreeNode.GetNodeDataIndex()];
        ImGuiTreeNodeFlags                      NodeFlags;
        if (_TreeNode.IsLeaf())
        {
            NodeFlags = NodeData.GetIsSelected() ? LeafFlags | ImGuiTreeNodeFlags_Selected : LeafFlags;
        }
        else
        {
            NodeFlags = NodeData.GetIsSelected() ? BaseFlags | ImGuiTreeNodeFlags_Selected : BaseFlags;
        }

        const bool NodeOpen = ImGui::TreeNodeEx(&NodeData, NodeFlags, TCHAR_TO_ANSI(*NodeData.GetImGuiNodeString()));
        return NodeOpen;
    };

    auto NodeAlreadyVisistedLambda = [](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        ImGui::TreePop();
    };

    auto EvaluateNodeLambda = [](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        if (_TreeNode.IsLeaf())
        {
            ImGui::TreePop();
        }
        else
        {
        }
    };

    KL::Debug::ImGuiVisualizer::Tree::Helpers::PreoderTraversalImGui(mTreeNodes, KeepTraversingTreeLambda, NodeAlreadyVisistedLambda, EvaluateNodeLambda);
}

void FKLDebugImGuiFeatureVisualizerTree::GatherAndSortFeatures(FKLDebugImGuiFeaturesIterator& _Iterator, TArray<FKLDebugImGuiVisualizerTreeSortedFeatures>& _FeaturesSorted) const
{
    _FeaturesSorted.Reserve(_Iterator.GetFeaturesCount());

    for (; _Iterator; ++_Iterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = _Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        _FeaturesSorted.Emplace(_Iterator.GetFeatureDataIndex(), _Iterator.GetFeatureData(), FeatureInterface.GetImGuiPath());
    }

    _FeaturesSorted.Sort([](const FKLDebugImGuiVisualizerTreeSortedFeatures& _Left, const FKLDebugImGuiVisualizerTreeSortedFeatures& _Right) -> bool {
        return _Left.GetImGuiPath().LexicalLess(_Right.GetImGuiPath());
    });
}

void FKLDebugImGuiFeatureVisualizerTree::GenerateTree(const TArray<FKLDebugImGuiVisualizerTreeSortedFeatures>& _FeaturesSorted)
{
    // example of sorted list
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

    mTreeNodes.Reserve(_FeaturesSorted.Num());
    mNodesData.Reserve(_FeaturesSorted.Num());

    FString TempPath;
    TempPath.Reserve(300);
    TArray<FName> PathTokens;
    PathTokens.Reserve(20);

    TArray<FKLDebugImGuiFeatureVisualizerStackData> NodesStack;
    NodesStack.Reserve(20);

    for (const FKLDebugImGuiVisualizerTreeSortedFeatures& Feature : _FeaturesSorted)
    {
        if (!NodesStack.IsEmpty())
        {
            const FKLDebugImGuiFeatureVisualizerStackData& LastStackNode = NodesStack.Last();
            if (LastStackNode.GetFullPath() == Feature.GetImGuiPath())
            {
                LastStackNode.GetTreeNodeMutable(mTreeNodes).AddFeatureDataIndex(Feature.GetFeatureDataIndex());
                continue;
            }
        }

        const FKLDebugImGuiFeatureData& FeatureData = Feature.GetFeatureData();

        const int32                             StopIndex  = FindMatchingParent(FeatureData.GetImGuiPathTokens(), NodesStack);
        FKLDebugImGuiFeatureVisualizerTreeNode* ParentNode = nullptr;

        for (int32 i = StopIndex + 1; i < FeatureData.GetImGuiPathTokens().Num() - 1; ++i)
        {
            const FName& ImGuiPath = FeatureData.GetImGuiPathTokens()[i];

            mNodesData.Emplace(ImGuiPath);

            if (ParentNode)
            {
                ParentNode->TrySetFirstChildIndex(mTreeNodes.Num());
            }

            FKLDebugImGuiFeatureVisualizerTreeNode& NewTreeNode = mTreeNodes.Emplace_GetRef(mNodesData.Num() - 1);
            ParentNode                                          = &NewTreeNode;

            TArrayView<const FName> SubPath     = MakeArrayView<const FName>(&FeatureData.GetImGuiPathTokens()[0], i + 1);
            const FName             FullSubPath = GetImGuiPathName(SubPath, TempPath);

            NodesStack.Emplace(FullSubPath, ImGuiPath, mTreeNodes.Num() - 1);
        }

        if (ParentNode)
        {
            ParentNode->TrySetFirstChildIndex(mTreeNodes.Num());
        }

        const FName& ImGuiPath = FeatureData.GetImGuiPathTokens().Last();
        mNodesData.Emplace(ImGuiPath);
        FKLDebugImGuiFeatureVisualizerTreeNode& NewTreeNode = mTreeNodes.Emplace_GetRef(mNodesData.Num() - 1);
        NewTreeNode.AddFeatureDataIndex(Feature.GetFeatureDataIndex());
        NodesStack.Emplace(Feature.GetImGuiPath(), ImGuiPath, mTreeNodes.Num() - 1);
    }

    mTreeNodes.Shrink();
    mNodesData.Shrink();
}

int32 FKLDebugImGuiFeatureVisualizerTree::FindMatchingParent(const TArray<FName>& _ImGuiPathTokens, TArray<FKLDebugImGuiFeatureVisualizerStackData>& _TreeNodesStack)
{
    FKLDebugImGuiFeatureVisualizerTreeNode* LastNodeInSameTreeLevel = nullptr;
    const int32                             Count                   = FMath::Min(_ImGuiPathTokens.Num(), _TreeNodesStack.Num());
    int32                                   StopIndex               = -1;

    if (Count == 0)
    {
        ensureMsgf(_TreeNodesStack.IsEmpty(), TEXT("imguiu path with no elements"));
        return StopIndex;
    }

    for (int32 i = 0; i < Count; ++i)
    {
        FKLDebugImGuiFeatureVisualizerStackData& StackData   = _TreeNodesStack[i];
        const FName&                             TokenName   = _ImGuiPathTokens[i];
        FKLDebugImGuiFeatureVisualizerTreeNode&  NodeInStack = StackData.GetTreeNodeMutable(mTreeNodes);

        if (StackData.GetNodeName() == TokenName)
        {
            StopIndex = i;

            LastNodeInSameTreeLevel = &NodeInStack;
            continue;
        }

        NodeInStack.SetNextTreeLevelNodeIndex(mTreeNodes.Num());
        const int32 RemoveCount = _TreeNodesStack.Num() - i;
        _TreeNodesStack.RemoveAt(i, RemoveCount, false);
        return StopIndex;
    }

    if (LastNodeInSameTreeLevel)
    {
        LastNodeInSameTreeLevel->TrySetFirstChildIndex(mTreeNodes.Num());
    }

    return StopIndex;
}

FName FKLDebugImGuiFeatureVisualizerTree::GetImGuiPathName(const TArrayView<const FName>& _ImGuiSubPath, FString& _String) const
{
    _String.Reset();
    if (_ImGuiSubPath.IsEmpty())
    {
        ensureMsgf(false, TEXT("we should have a path"));
        return FName();
    }

    for (int32 i = 0; i < _ImGuiSubPath.Num() - 1; ++i)
    {
        const FName& Node = _ImGuiSubPath[i];
        _String.Appendf(TEXT("%s."), *Node.ToString());
    }

    _String.Appendf(TEXT("%s"), *_ImGuiSubPath.Last().ToString());
    return FName(_String);
}