#pragma once

#include "TreeBuilder/KLDebugImGuiTreeBuilderData.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderNode.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderStackData.h"

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"
#include "UObject/NameTypes.h"

namespace KL::Debug::ImGuiTreeBuilder::Helpers
{
    namespace Private
    {
        template<typename TreeNodeType>
        using NodesStackPair = TPair<bool, const TreeNodeType*>;

        template<typename TreeNodeType, typename KeepTraversingCallback, typename OperateOnNodeAlreadyVisisted, typename OperateOnNodeCallback, typename OperateBeforeAddingChildCallback>
        void PreoderTraversalInternal(const bool _CheckNodeAlreadyVisisted, const TArray<TreeNodeType>& _TreeNodes, const KeepTraversingCallback& _KeepTraversingCbk, const OperateOnNodeAlreadyVisisted& _NodeAlreadyVisistedCbk, const OperateOnNodeCallback& _OperateOnNodeCbk, const OperateBeforeAddingChildCallback& _OperateBeforeAddingChildCallback)
        {
            if (_TreeNodes.IsEmpty())
            {
                return;
            }

            TArray<NodesStackPair<TreeNodeType>> NodesStack;
            NodesStack.Reserve(_TreeNodes.Num());
            NodesStack.Emplace(NodesStackPair<TreeNodeType>(false, &_TreeNodes[0]));

            while (!NodesStack.IsEmpty())
            {
                NodesStackPair<TreeNodeType> CurrentStackNode = NodesStack.Pop(false);
                const TreeNodeType*          TreeNode         = CurrentStackNode.Value;

                if (_CheckNodeAlreadyVisisted && CurrentStackNode.Key)
                {
                    _NodeAlreadyVisistedCbk(*TreeNode);
                    continue;
                }

                if (!_KeepTraversingCbk(*TreeNode))
                {
                    // this subtree is not open, try to see if the neighbor one is open
                    const TOptional<uint16> NextNode = TreeNode->GetNextTreeLevelNodeIndex();
                    if (NextNode.IsSet())
                    {
                        NodesStack.Emplace(NodesStackPair<TreeNodeType>(false, &_TreeNodes[NextNode.GetValue()]));
                    }

                    continue;
                }

                _OperateOnNodeCbk(*static_cast<const TreeNodeType*>(TreeNode));

                // add the neighbor node
                const TOptional<uint16> NextNode = TreeNode->GetNextTreeLevelNodeIndex();
                if (NextNode.IsSet())
                {
                    NodesStack.Emplace(NodesStackPair<TreeNodeType>(false, &_TreeNodes[NextNode.GetValue()]));
                }

                if (!TreeNode->IsLeaf())
                {
                    _OperateBeforeAddingChildCallback(*TreeNode, NodesStack);

                    // add the child if any
                    const uint16 ChildIndex = TreeNode->GetFirstChildIndex();
                    NodesStack.Emplace(NodesStackPair<TreeNodeType>(false, &_TreeNodes[ChildIndex]));
                }
            }
        }

        template<typename TreeNodeType>
        int32 FindMatchingParent(const TArray<FName>& _PathTokens, TArray<FKLDebugImGuiTreeBuilderStackData>& _TreeNodesStack, TArray<TreeNodeType>& _TreeNodes)
        {
            TreeNodeType* LastNodeInSameTreeLevel = nullptr;
            const int32   Count                   = FMath::Min(_PathTokens.Num(), _TreeNodesStack.Num());
            int32         StopIndex               = -1;

            if (Count == 0)
            {
                ensureMsgf(_TreeNodesStack.IsEmpty(), TEXT("path with no elements"));
                return StopIndex;
            }

            for (int32 i = 0; i < Count; ++i)
            {
                FKLDebugImGuiTreeBuilderStackData& StackData     = _TreeNodesStack[i];
                const FName&                       TokenName     = _PathTokens[i];
                const uint16                       TreeNodeIndex = StackData.GetTreeNodeIndex();
                TreeNodeType&                      NodeInStack   = _TreeNodes[TreeNodeIndex];

                if (StackData.GetNodeName() == TokenName)
                {
                    StopIndex = i;

                    LastNodeInSameTreeLevel = &NodeInStack;
                    continue;
                }

                NodeInStack.SetNextTreeLevelNodeIndex(_TreeNodes.Num());
                const int32 RemoveCount = _TreeNodesStack.Num() - i;
                _TreeNodesStack.RemoveAt(i, RemoveCount, false);
                return StopIndex;
            }

            if (LastNodeInSameTreeLevel)
            {
                LastNodeInSameTreeLevel->TrySetFirstChildIndex(_TreeNodes.Num());
            }

            return StopIndex;
        }
    }  // namespace Private

    template<typename TreeNodeType, typename KeepTraversingCallback, typename OperateOnNodeCallback>
    void PreoderTraversal(const TArray<TreeNodeType>& _TreeNodes, const KeepTraversingCallback& _KeepTraversingCbk, const OperateOnNodeCallback& _OperateOnNodeCbk)
    {
        auto OperateOnNodeAlreadyVisistedLambda = [](const TreeNodeType& _TreeNode) -> void {
        };

        auto Lambda = [](const TreeNodeType& TreeNode, TArray<Private::NodesStackPair<TreeNodeType>>& _NodeStack) -> void {

        };

        Private::PreoderTraversalInternal(false, _TreeNodes, _KeepTraversingCbk, OperateOnNodeAlreadyVisistedLambda, _OperateOnNodeCbk, Lambda);
    }

    template<typename TreeNodeType, typename KeepTraversingCallback, typename OperateOnNodeAlreadyVisisted, typename OperateOnNodeCallback>
    void PreoderTraversalImGui(const TArray<TreeNodeType>& _TreeNodes, const KeepTraversingCallback& _KeepTraversingCbk, const OperateOnNodeAlreadyVisisted& _NodeAlreadyVisitedCbk, const OperateOnNodeCallback& _OperateOnNodeCbk)
    {
        auto CloseParentLambda = [](const TreeNodeType& TreeNode, TArray<Private::NodesStackPair<TreeNodeType>>& _NodeStack) -> void {
            _NodeStack.Emplace(Private::NodesStackPair<TreeNodeType>(true, &TreeNode));
        };

        Private::PreoderTraversalInternal(true, _TreeNodes, _KeepTraversingCbk, _NodeAlreadyVisitedCbk, _OperateOnNodeCbk, CloseParentLambda);
    }

    FName GetFullPathName(const TArrayView<const FName>& _ImGuiSubPath, FString& _String)
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

    template<typename FeatureSortedType, typename TreeNodeType, typename CheckSameStackNodeDataCbk, typename OnGenerateNodeCbk, typename OnGenerateLeafCbk>
    void GenerateTree(const TArray<FeatureSortedType>& _FeaturesSorted, TArray<TreeNodeType>& _TreeNodes, const CheckSameStackNodeDataCbk& _CheckSameNodeDataCbk, const OnGenerateNodeCbk& _OnGenerateNodeCbk, const OnGenerateLeafCbk& _OnGenerateLeafCbk)
    {
        static constexpr uint8 MaxTreeDepth = 40;

        FString TempPath;
        TempPath.Reserve(300);

        TArray<FKLDebugImGuiTreeBuilderStackData> NodesStack;
        NodesStack.Reserve(MaxTreeDepth);

        for (const FeatureSortedType& Feature : _FeaturesSorted)
        {
            if (!NodesStack.IsEmpty())
            {
                const FKLDebugImGuiTreeBuilderStackData& LastStackNode = NodesStack.Last();
                if (_CheckSameNodeDataCbk(LastStackNode, Feature))
                {
                    continue;
                }
            }

            const TArray<FName>& PathTokens = Feature.GetPathTokens();
            const int32          StopIndex  = Private::FindMatchingParent(PathTokens, NodesStack, _TreeNodes);
            TreeNodeType*        ParentNode = nullptr;

            for (int32 i = StopIndex + 1; i < PathTokens.Num() - 1; ++i)
            {
                const FName& PathToken = PathTokens[i];

                if (ParentNode)
                {
                    ParentNode->TrySetFirstChildIndex(_TreeNodes.Num());
                }

                TreeNodeType& NewTreeNode = _OnGenerateNodeCbk(PathToken);
                ParentNode                = &NewTreeNode;

                TArrayView<const FName> SubPath     = MakeArrayView<const FName>(&PathTokens[0], i + 1);
                const FName             FullSubPath = GetFullPathName(SubPath, TempPath);

                NodesStack.Emplace(FullSubPath, PathToken, _TreeNodes.Num() - 1);
            }

            if (ParentNode)
            {
                ParentNode->TrySetFirstChildIndex(_TreeNodes.Num());
            }

            const FName& LastPathToken = PathTokens.Last();
            _OnGenerateLeafCbk(LastPathToken, Feature);
            NodesStack.Emplace(Feature.GetFullPath(), LastPathToken, _TreeNodes.Num() - 1);
        }

        _TreeNodes.Shrink();
    }
}  // namespace KL::Debug::ImGuiTreeBuilder::Helpers