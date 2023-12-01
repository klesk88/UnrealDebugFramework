// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Filter/Tree/KLDebugImGuiFilterTreeNode.h"

FKLDebugImGuiFilterTreeNode::FKLDebugImGuiFilterTreeNode(const KL::Debug::ImGui::Features::Types::FilterIndex _FilterIndex)
    : FKLDebugImGuiTreeBuilderNode()
    , mFilterIndex(_FilterIndex)
{
}

FKLDebugImGuiFilterTreeNode::FKLDebugImGuiFilterTreeNode(const int32 _NodeDataIndex, const KL::Debug::ImGui::Features::Types::FilterIndex _FilterIndex)
    : FKLDebugImGuiTreeBuilderNode(_NodeDataIndex)
    , mFilterIndex(_FilterIndex)
{
}
