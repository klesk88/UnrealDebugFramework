// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerNodeData.h"

// engine
#include "Math/NumericLimits.h"

namespace KL::Debug::ImGui::Features::VisualizerTreeData
{
    KL::Debug::ImGui::Features::VisualizerTree::NodeDataID ID = 0;

    [[nodiscard]] uint16 GetNextID()
    {
        ID = static_cast<KL::Debug::ImGui::Features::VisualizerTree::NodeDataID>((static_cast<uint32>(ID) + 1) % TNumericLimits<KL::Debug::ImGui::Features::VisualizerTree::NodeDataID>::Max());
        return ID;
    }
}    // namespace KL::Debug::ImGui::Features::VisualizerTreeData

/////////////////////////////////////////////////////////////////////////////

FKLDebugImGuiFeatureVisualizerNodeData::FKLDebugImGuiFeatureVisualizerNodeData(const FName& _Name)
    : mNameAsString(_Name.ToString())
    , mID(KL::Debug::ImGui::Features::VisualizerTreeData::GetNextID())
{
}
