// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeTypes.h"

// engine
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerNodeData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerNodeData(const FName& _Name);
    [[nodiscard]] bool operator==(const uint16 _NodeDataID) const;

    [[nodiscard]] const FString& GetImGuiNodeString() const;

    [[nodiscard]] bool GetIsSelected() const;
    [[nodiscard]] KL::Debug::ImGui::Features::VisualizerTree::NodeDataID GetID() const;

    void ClearIsSelected();
    void ToogleIsSelected();

private:
    FString mNameAsString;
    bool mIsSelected = false;
    KL::Debug::ImGui::Features::VisualizerTree::NodeDataID mID = 0;
};

inline bool FKLDebugImGuiFeatureVisualizerNodeData::operator==(const uint16 _NodeDataID) const
{
    return mID == _NodeDataID;
}

inline const FString& FKLDebugImGuiFeatureVisualizerNodeData::GetImGuiNodeString() const
{
    return mNameAsString;
}

inline bool FKLDebugImGuiFeatureVisualizerNodeData::GetIsSelected() const
{
    return mIsSelected;
}

inline void FKLDebugImGuiFeatureVisualizerNodeData::ClearIsSelected()
{
    mIsSelected = false;
}

inline void FKLDebugImGuiFeatureVisualizerNodeData::ToogleIsSelected()
{
    mIsSelected = !mIsSelected;
}

inline KL::Debug::ImGui::Features::VisualizerTree::NodeDataID FKLDebugImGuiFeatureVisualizerNodeData::GetID() const
{
    return mID;
}