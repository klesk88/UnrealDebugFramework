#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiVisualizerTreeSortedFeatures.h"
#include "Helpers/KLDebugImGuiHelpers.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderHelpers.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderStackData.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

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

void FKLDebugImGuiFeatureVisualizerTree::DrawImGuiTree(TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexesSelected)
{
    static constexpr ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static constexpr ImGuiTreeNodeFlags LeafFlags = BaseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    auto KeepTraversingTreeLambda = [this, &_FeaturesIndexesSelected](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> bool {
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
            NodeFlags = LeafFlags;
        }
        else
        {
            NodeFlags = BaseFlags;
        }

        if (NodeData.GetIsSelected())
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        if (_TreeNode.HasFeatures())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, KL::Debug::ImGuiHelpers::ConvertUnrealColorToImGui(FColor::Yellow));
        }

        const bool NodeOpen = ImGui::TreeNodeEx(&NodeData, NodeFlags, TCHAR_TO_ANSI(*NodeData.GetImGuiNodeString()));

        if (_TreeNode.HasFeatures())
        {
            ImGui::PopStyleColor(1);
        }

        if (_TreeNode.HasFeatures() && ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            NodeData.ToogleIsSelected();
            if (NodeData.GetIsSelected())
            {
                for (const KL::Debug::ImGui::Features::Types::FeatureIndex FeatureIndex : _TreeNode.GetFeatureIndexes())
                {
                    ensureMsgf(_FeaturesIndexesSelected.IndexOfByKey(FeatureIndex) == INDEX_NONE, TEXT("adding same feature multiple times"));
                    _FeaturesIndexesSelected.Emplace(FeatureIndex, NodeData.GetID());
                }
            }
            else
            {
                for (const KL::Debug::ImGui::Features::Types::FeatureIndex FeatureIndex : _TreeNode.GetFeatureIndexes())
                {
                    const int32 Index = _FeaturesIndexesSelected.IndexOfByKey(FeatureIndex);
#if DO_ENSURE
                    if (Index == INDEX_NONE)
                    {
                        ensureMsgf(false, TEXT("feature not found"));
                        continue;
                    }
#endif

                    _FeaturesIndexesSelected.RemoveAtSwap(Index, 1, false);
                }
            }
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

void FKLDebugImGuiFeatureVisualizerTree::ClearToogleNodeData(const KL::Debug::ImGui::Features::VisualizerTree::NodeDataID _NodeDataID)
{
    const int32 Index = mNodesData.IndexOfByKey(_NodeDataID);
    if (Index == INDEX_NONE)
    {
        ensureMsgf(false, TEXT("element not found"));
        return;
    }

    FKLDebugImGuiFeatureVisualizerNodeData& NodeData = mNodesData[Index];
    NodeData.ClearIsSelected();
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

    checkf(mNodesData.Num() < TNumericLimits<KL::Debug::ImGui::Features::VisualizerTree::NodeDataID>::Max(), TEXT("the node data ID supports currently until uint16"));
}
