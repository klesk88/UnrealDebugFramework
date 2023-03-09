#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeNode.h"

FKLDebugImGuiFeatureVisualizerTreeNode::FKLDebugImGuiFeatureVisualizerTreeNode(const int32 _NodeDataIndex)
    : mNodeDataIndex(static_cast<uint16>(_NodeDataIndex))
{
    checkf(_NodeDataIndex < TNumericLimits<uint16>::Max(), TEXT("too many data nodes"));
    mFeaturesIndexes.Reserve(5);
}
