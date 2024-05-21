// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeTypes.h"

// engine
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"

class IKLDebugContextInterface;
class IKLDebugFeatureInterfaceBase;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerEntry
{
public:
    explicit FKLDebugImGuiFeatureVisualizerEntry(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeaturesIndex, const KL::Debug::ImGui::Features::VisualizerTree::NodeDataID _NodeDataID, TUniquePtr<IKLDebugContextInterface>&& _FeatureContext);
    ~FKLDebugImGuiFeatureVisualizerEntry();

    [[nodiscard]] bool operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const;

    [[nodiscard]] KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureIndex() const;
    [[nodiscard]] KL::Debug::ImGui::Features::VisualizerTree::NodeDataID GetNodeDataID() const;

    [[nodiscard]] bool& GetIsEnableRef();
    [[nodiscard]] bool IsEnable() const;

    [[nodiscard]] IKLDebugContextInterface* TryGetFeatureContextMutable() const;

protected:
    TUniquePtr<IKLDebugContextInterface> mFeatureContext;
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
