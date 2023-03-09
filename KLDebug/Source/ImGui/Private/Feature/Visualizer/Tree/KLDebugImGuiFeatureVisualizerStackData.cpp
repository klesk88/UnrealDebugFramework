#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerStackData.h"

// engine
#include "Math/NumericLimits.h"

FKLDebugImGuiFeatureVisualizerStackData::FKLDebugImGuiFeatureVisualizerStackData(const FName& _FullPath, const FName& _NodeName, const int32 _TreeNodeIdx)
    : mFullPath(_FullPath)
    , mNodeName(_NodeName)
    , mTreeNode(static_cast<uint16>(_TreeNodeIdx))
{
    checkf(_TreeNodeIdx < TNumericLimits<uint16>::Max(), TEXT("too manny tree nodes"));
}
