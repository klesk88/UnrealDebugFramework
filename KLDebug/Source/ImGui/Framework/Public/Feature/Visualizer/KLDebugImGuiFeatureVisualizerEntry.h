#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeTypes.h"

// engine
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"

class FKLDebugImGuiFeatureContext_Base;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerEntry
{
public:
    explicit FKLDebugImGuiFeatureVisualizerEntry(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeaturesIndex, const KL::Debug::ImGui::Features::VisualizerTree::NodeDataID _NodeDataID, TUniquePtr<FKLDebugImGuiFeatureContext_Base>&& _FeatureContext);
    ~FKLDebugImGuiFeatureVisualizerEntry();

    UE_NODISCARD bool operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const;

    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureIndex() const;
    UE_NODISCARD KL::Debug::ImGui::Features::VisualizerTree::NodeDataID GetNodeDataID() const;

    UE_NODISCARD bool& GetIsEnableRef();
    UE_NODISCARD bool IsEnable() const;

    UE_NODISCARD FKLDebugImGuiFeatureContext_Base* TryGetFeatureContextMutable() const;

protected:
    TUniquePtr<FKLDebugImGuiFeatureContext_Base> mFeatureContext;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex;
    KL::Debug::ImGui::Features::VisualizerTree::NodeDataID mNodeDataID = 0;
    bool mIsEnable = true;
};

inline bool FKLDebugImGuiFeatureVisualizerEntry::operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const
{
    return mFeatureIndex == _FeatureIndex;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFeatureVisualizerEntry::GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline KL::Debug::ImGui::Features::VisualizerTree::NodeDataID FKLDebugImGuiFeatureVisualizerEntry::GetNodeDataID() const
{
    return mNodeDataID;
}

inline bool& FKLDebugImGuiFeatureVisualizerEntry::GetIsEnableRef()
{
    return mIsEnable;
}

inline bool FKLDebugImGuiFeatureVisualizerEntry::IsEnable() const
{
    return mIsEnable;
}
