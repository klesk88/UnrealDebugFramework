// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiVisualizerTreeSortedFeatures.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderHelpers.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderStackData.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "ImGui/User/Public/Helpers/KLDebugImGuiHelpers.h"
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

void FKLDebugImGuiFeatureVisualizerTree::DrawImGuiTree(const EImGuiInterfaceType _ContainerType, const FKLDebugImGuiFeatureContextInput& _ContextInput, const FKLDebugImGuiFeatureVisualizerImGuiContext& _ImguiContext, TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexesSelected)
{
    static constexpr ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static constexpr ImGuiTreeNodeFlags LeafFlags = BaseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    const FKLDebugImGuiFeatureContainerBase& FeatureContainer = _ImguiContext.GetFeaturesContainerManager().GetContainer(_ContainerType);

    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> FeaturesUpdated;
    if (_ImguiContext.GetFeatureUpdateDelegate().IsBound())
    {
        FeaturesUpdated.Reserve(30);
    }

    auto KeepTraversingTreeLambda = [this, &_FeaturesIndexesSelected, &FeatureContainer, &_ContextInput, &_ImguiContext, &FeaturesUpdated, _ContainerType](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> bool {
        const TOptional<uint16> NodeDataIndex = _TreeNode.GetNodeDataIndex();
        if (!NodeDataIndex.IsSet())
        {
            ensureMsgf(false, TEXT("we should always have valid data"));
            return false;
        }

        FKLDebugImGuiFeatureVisualizerNodeData& NodeData = mNodesData[NodeDataIndex.GetValue()];
        ImGuiTreeNodeFlags NodeFlags;
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

        FeaturesUpdated.Reset();
        bool IsAdded = false;

        if (_TreeNode.HasFeatures() && ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            NodeData.ToogleIsSelected();
            if (NodeData.GetIsSelected())
            {
                for (const KL::Debug::ImGui::Features::Types::FeatureIndex FeatureIndex : _TreeNode.GetFeatureIndexes())
                {
                    ensureMsgf(_FeaturesIndexesSelected.IndexOfByKey(FeatureIndex) == INDEX_NONE, TEXT("adding same feature multiple times"));
                    const IKLDebugImGuiFeatureInterfaceBase& NewFeatureInterface = FeatureContainer.GetFeature(FeatureIndex);

                    {
                        TUniquePtr<FKLDebugImGuiFeatureContext_Base> Context = NewFeatureInterface.GetFeatureContext(_ContextInput);
                        _FeaturesIndexesSelected.Emplace(FeatureIndex, NodeData.GetID(), MoveTemp(Context));
                    }

                    FeaturesUpdated.Emplace(FeatureIndex);
                    IsAdded = true;
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
                    IsAdded = false;
                    FeaturesUpdated.Emplace(FeatureIndex);
                }
            }

            const FOnImGuiFeatureStateUpdated& Delegate = _ImguiContext.GetFeatureUpdateDelegate();
            if (Delegate.IsBound() && !FeaturesUpdated.IsEmpty())
            {
                FKLDebugImGuiSubsetFeaturesConstIterator Iterator = FeatureContainer.GetFeaturesSubsetConstIterator(FeaturesUpdated);
                FKLDebugImGuiFeatureStatusUpdateData DelegateData{ _ImguiContext.GetWorld(), IsAdded, _ContainerType, _ContextInput.GetObject(), Iterator };
                if (_FeaturesIndexesSelected.IsEmpty())
                {
                    DelegateData.SetFullyRemoved();
                }

                Delegate.Broadcast(DelegateData);
            }
        }

        return NodeOpen;
    };

    auto NodeAlreadyVisistedLambda = [](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        ImGui::TreePop();
    };

    auto EvaluateNodeLambda = [](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
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
            const uint16 TreeNodeIndex = _LastNodeInStack.GetTreeNodeIndex();
            FKLDebugImGuiFeatureVisualizerTreeNode& TreeNode = mTreeNodes[TreeNodeIndex];
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
