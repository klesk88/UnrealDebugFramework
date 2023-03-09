#pragma once

#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeNode.h"

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"

namespace KL::Debug::ImGuiTreeBuilder::Helpers
{
    namespace Private
    {
        using NodesStackPair = TPair<bool, const FKLDebugImGuiFeatureVisualizerTreeNode*>;

        template<typename KeepTraversingCallback, typename OperateOnNodeAlreadyVisisted, typename OperateOnNodeCallback, typename OperateBeforeAddingChildCallback>
        void PreoderTraversalInternal(const bool _CheckNodeAlreadyVisisted, const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& _TreeNodes, const KeepTraversingCallback& _KeepTraversingCbk, const OperateOnNodeAlreadyVisisted& _NodeAlreadyVisistedCbk, const OperateOnNodeCallback& _OperateOnNodeCbk, const OperateBeforeAddingChildCallback& _OperateBeforeAddingChildCallback)
        {
            if (_TreeNodes.IsEmpty())
            {
                return;
            }

            TArray<NodesStackPair> NodesStack;
            NodesStack.Reserve(_TreeNodes.Num());
            NodesStack.Emplace(NodesStackPair(false, &_TreeNodes[0]));

            while (!NodesStack.IsEmpty())
            {
                NodesStackPair                                CurrentStackNode = NodesStack.Pop(false);
                const FKLDebugImGuiFeatureVisualizerTreeNode* TreeNode         = CurrentStackNode.Value;

                if (!_KeepTraversingCbk(*TreeNode))
                {
                    // this subtree is not open, try to see if the neighbor one is open
                    const TOptional<uint16> NextNode = TreeNode->GetNextTreeLevelNodeIndex();
                    if (NextNode.IsSet())
                    {
                        NodesStack.Emplace(NodesStackPair(false, &_TreeNodes[NextNode.GetValue()]));
                    }

                    continue;
                }

                if (_CheckNodeAlreadyVisisted && CurrentStackNode.Key)
                {
                    _NodeAlreadyVisistedCbk(*TreeNode);
                    continue;
                }

                _OperateOnNodeCbk(*TreeNode);

                // add the neighbor node
                const TOptional<uint16> NextNode = TreeNode->GetNextTreeLevelNodeIndex();
                if (NextNode.IsSet())
                {
                    NodesStack.Emplace(NodesStackPair(false, &_TreeNodes[NextNode.GetValue()]));
                }

                if (!TreeNode->IsLeaf())
                {
                    _OperateBeforeAddingChildCallback(*TreeNode, NodesStack);

                    // add the child if any
                    const uint16 ChildIndex = TreeNode->GetFirstChildIndex();
                    NodesStack.Emplace(NodesStackPair(false, &_TreeNodes[ChildIndex]));
                }
            }
        }
    }  // namespace Private

    template<typename KeepTraversingCallback, typename OperateOnNodeCallback>
    void PreoderTraversal(const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& _TreeNodes, const KeepTraversingCallback& _KeepTraversingCbk, const OperateOnNodeCallback& _OperateOnNodeCbk)
    {
        auto OperateOnNodeAlreadyVisistedLambda = [](const FKLDebugImGuiFeatureVisualizerTreeNode& _TreeNode) -> void {
        };

        auto Lambda = [](const FKLDebugImGuiFeatureVisualizerTreeNode& TreeNode, TArray<Private::NodesStackPair>& _NodeStack) -> void {

        };

        Private::PreoderTraversalInternal(false, _TreeNodes, _KeepTraversingCbk, OperateOnNodeAlreadyVisistedLambda, _OperateOnNodeCbk, Lambda);
    }

    template<typename KeepTraversingCallback, typename OperateOnNodeAlreadyVisisted, typename OperateOnNodeCallback>
    void PreoderTraversalImGui(const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& _TreeNodes, const KeepTraversingCallback& _KeepTraversingCbk, const OperateOnNodeAlreadyVisisted& _NodeAlreadyVisitedCbk, const OperateOnNodeCallback& _OperateOnNodeCbk)
    {
        auto CloseParentLambda = [](const FKLDebugImGuiFeatureVisualizerTreeNode& TreeNode, TArray<Private::NodesStackPair>& _NodeStack) -> void {
            _NodeStack.Emplace(Private::NodesStackPair(true, &TreeNode));
        };

        Private::PreoderTraversalInternal(true, _TreeNodes, _KeepTraversingCbk, _NodeAlreadyVisitedCbk, _OperateOnNodeCbk, CloseParentLambda);
    }
}  // namespace KL::Debug::ImGuiTreeBuilder::Helpers