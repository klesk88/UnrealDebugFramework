#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiVisualizerTreeSortedFeatures.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderHelpers.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderStackData.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"
#include "Math/UnrealMathUtility.h"

void FKLDebugImGuiFeatureVisualizerTree::CreateTree(FKLDebugImGuiFeaturesConstIterator& _Iterator)
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
        const TOptional<uint16> NodeDataIndex = _TreeNode.GetNodeDataIndex();
        if (!NodeDataIndex.IsSet())
        {
            ensureMsgf(false, TEXT("we should always have valid data"));
            return false;
        }

        FKLDebugImGuiFeatureVisualizerNodeData& NodeData = mNodesData[NodeDataIndex.GetValue()];
        ImGuiTreeNodeFlags                      NodeFlags;
        if (_TreeNode.IsLeaf())
        {
            NodeFlags |= LeafFlags;
        }

        if (_TreeNode.HasFeatures())
        {
            NodeFlags = NodeData.GetIsSelected() ? LeafFlags | ImGuiTreeNodeFlags_Selected : LeafFlags;
        }

        const bool NodeOpen = ImGui::TreeNodeEx(&NodeData, NodeFlags, TCHAR_TO_ANSI(*NodeData.GetImGuiNodeString()));
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            NodeData.SetIsSelected();
        }
        else
        {
            NodeData.ClearIsSelected();
        }

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

    KL::Debug::ImGuiTreeBuilder::Helpers::PreoderTraversalImGui(mTreeNodes, KeepTraversingTreeLambda, NodeAlreadyVisistedLambda, EvaluateNodeLambda);
}

void FKLDebugImGuiFeatureVisualizerTree::GatherAndSortFeatures(FKLDebugImGuiFeaturesConstIterator& _Iterator, TArray<FKLDebugImGuiVisualizerTreeSortedFeatures>& _FeaturesSorted) const
{
    _FeaturesSorted.Reserve(_Iterator.GetFeaturesCount());

    for (; _Iterator; ++_Iterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = _Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        _FeaturesSorted.Emplace(_Iterator.GetFeatureDataIndex(), _Iterator.GetFeatureData(), FeatureInterface.GetImGuiPath());
    }

    _FeaturesSorted.Sort([](const FKLDebugImGuiVisualizerTreeSortedFeatures& _Left, const FKLDebugImGuiVisualizerTreeSortedFeatures& _Right) -> bool {
        return _Left.GetFullPath().LexicalLess(_Right.GetFullPath());
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

    auto IsSameDataLambda = [this](const FKLDebugImGuiTreeBuilderStackData& _LastNodeInStack, const FKLDebugImGuiVisualizerTreeSortedFeatures& _Feature) -> bool {
        if (_LastNodeInStack.GetFullPath() == _Feature.GetFullPath())
        {
            const uint16                            TreeNodeIndex = _LastNodeInStack.GetTreeNodeIndex();
            FKLDebugImGuiFeatureVisualizerTreeNode& TreeNode      = mTreeNodes[TreeNodeIndex];
            TreeNode.AddFeatureDataIndex(_Feature.GetFeatureDataIndex());
            return true;
        }

        return false;
    };

    auto AllocateTreeNode = [this](const FName& _FilterToken) -> FKLDebugImGuiFeatureVisualizerTreeNode& {
        mNodesData.Emplace(_FilterToken);
        return mTreeNodes.Emplace_GetRef(mNodesData.Num() - 1);
    };

    auto AllocateTreeNodeLeaf = [this](const FName& _FilterToken, const FKLDebugImGuiVisualizerTreeSortedFeatures& _SortedFeature) -> FKLDebugImGuiFeatureVisualizerTreeNode& {
        mNodesData.Emplace(_FilterToken);
        FKLDebugImGuiFeatureVisualizerTreeNode& NewTreeNode = mTreeNodes.Emplace_GetRef(mNodesData.Num() - 1);
        NewTreeNode.AddFeatureDataIndex(_SortedFeature.GetFeatureDataIndex());
        return NewTreeNode;
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::GenerateTree(_FeaturesSorted, mTreeNodes, IsSameDataLambda, AllocateTreeNode, AllocateTreeNodeLeaf);

    mTreeNodes.Shrink();
    mNodesData.Shrink();
}
