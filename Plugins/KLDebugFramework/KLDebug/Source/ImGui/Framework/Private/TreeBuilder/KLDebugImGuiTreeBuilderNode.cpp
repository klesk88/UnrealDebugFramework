// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "TreeBuilder/KLDebugImGuiTreeBuilderNode.h"

FKLDebugImGuiTreeBuilderNode::FKLDebugImGuiTreeBuilderNode(const int32 _NodeDataIndex)
    : mNodeDataIndex(static_cast<uint16>(_NodeDataIndex))
{
    checkf(_NodeDataIndex < TNumericLimits<uint16>::Max(), TEXT("too many data nodes"));
}
