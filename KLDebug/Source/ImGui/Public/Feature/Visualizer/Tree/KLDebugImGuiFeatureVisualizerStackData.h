#pragma once

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiFeatureVisualizerTreeNode;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerStackData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerStackData(const FName& _FullPath, const FName& _NodeName, const int32 _TreeNodeIdx);

    UE_NODISCARD const FName&                            GetFullPath() const;
    UE_NODISCARD const FName&                            GetNodeName() const;
    UE_NODISCARD FKLDebugImGuiFeatureVisualizerTreeNode& GetTreeNodeMutable(TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& _TreeNodes) const;

private:
    FName  mFullPath;
    FName  mNodeName;
    uint16 mTreeNode;
};

inline const FName& FKLDebugImGuiFeatureVisualizerStackData::GetFullPath() const
{
    return mFullPath;
}

inline const FName& FKLDebugImGuiFeatureVisualizerStackData::GetNodeName() const
{
    return mNodeName;
}

inline FKLDebugImGuiFeatureVisualizerTreeNode& FKLDebugImGuiFeatureVisualizerStackData::GetTreeNodeMutable(TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& _TreeNode) const
{
    return _TreeNode[mTreeNode];
}
