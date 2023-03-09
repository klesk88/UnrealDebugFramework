#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerTreeNode final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerTreeNode(const int32 _NodeDataIndex);

    void AddFeatureDataIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex);
    void TrySetFirstChildIndex(const int32 _Index);
    void SetNextTreeLevelNodeIndex(const int32 _Index);

    // the node is not guarantee to have a valid next tree level node
    UE_NODISCARD TOptional<uint16> GetNextTreeLevelNodeIndex() const;
    UE_NODISCARD uint16            GetFirstChildIndex() const;
    UE_NODISCARD uint16            GetNodeDataIndex() const;
    UE_NODISCARD bool              IsLeaf() const;

private:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesIndexes;
    // this represents, in the tree, the next node location at the same level of this node
    uint16 mNextTreeLevelNode = TNumericLimits<uint16>::Max();
    // this represents, in the tree, the first child index
    uint16 mStartDataChildIdx = TNumericLimits<uint16>::Max();
    // this stored the data index associated with this node
    uint16 mNodeDataIndex = TNumericLimits<uint16>::Max();
};

inline void FKLDebugImGuiFeatureVisualizerTreeNode::AddFeatureDataIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex)
{
    mFeaturesIndexes.Emplace(_FeatureDataIndex);
}

inline void FKLDebugImGuiFeatureVisualizerTreeNode::TrySetFirstChildIndex(const int32 _Index)
{
    if (mStartDataChildIdx != TNumericLimits<uint16>::Max())
    {
        // we already set the node
        return;
    }

    checkf(_Index < TNumericLimits<uint16>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    mStartDataChildIdx = static_cast<uint16>(_Index);
}

inline uint16 FKLDebugImGuiFeatureVisualizerTreeNode::GetFirstChildIndex() const
{
    return mStartDataChildIdx;
}

inline void FKLDebugImGuiFeatureVisualizerTreeNode::SetNextTreeLevelNodeIndex(const int32 _Index)
{
    checkf(_Index < TNumericLimits<uint16>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    mNextTreeLevelNode = static_cast<uint16>(_Index);
}

inline TOptional<uint16> FKLDebugImGuiFeatureVisualizerTreeNode::GetNextTreeLevelNodeIndex() const
{
    if (mNextTreeLevelNode != TNumericLimits<uint16>::Max())
    {
        return TOptional<uint16>(mNextTreeLevelNode);
    }

    return TOptional<uint16>();
}

inline uint16 FKLDebugImGuiFeatureVisualizerTreeNode::GetNodeDataIndex() const
{
    checkf(mNodeDataIndex != TNumericLimits<uint16>::Max(), TEXT("must be set to correct value")) return mNodeDataIndex;
    return mNodeDataIndex;
}

inline bool FKLDebugImGuiFeatureVisualizerTreeNode::IsLeaf() const
{
    return mStartDataChildIdx == TNumericLimits<uint16>::Max();
}