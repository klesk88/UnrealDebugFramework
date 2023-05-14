#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerNodeData.h"

//engine
#include "Math/NumericLimits.h"

namespace KL::Debug::ImGui::Features::VisualizerTreeData
{
    KL::Debug::ImGui::Features::VisualizerTree::NodeDataID ID = 0;

    UE_NODISCARD uint16 GetNextID()
    {
        ID = static_cast<KL::Debug::ImGui::Features::VisualizerTree::NodeDataID>((static_cast<uint32>(ID) + 1) % TNumericLimits<KL::Debug::ImGui::Features::VisualizerTree::NodeDataID>::Max());
        return ID;
    }
}

/////////////////////////////////////////////////////////////////////////////

FKLDebugImGuiFeatureVisualizerNodeData::FKLDebugImGuiFeatureVisualizerNodeData(const FName& _Name)
    : mNameAsString(_Name.ToString())
    , mID(KL::Debug::ImGui::Features::VisualizerTreeData::GetNextID())
{
}
