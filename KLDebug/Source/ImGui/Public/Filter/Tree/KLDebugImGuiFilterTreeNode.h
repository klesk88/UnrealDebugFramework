#pragma once

#include "Feature/KLDebugFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFilterTreeNode final : public FNoncopyable
{
public:
    typedef uint16 ChildIndex;

public:
    explicit FKLDebugImGuiFilterTreeNode(const uint16 _FilterIndex);

    void SetNextTreeLevelNodeIndex(const int32 _Index);
    void SetStartDataOffset(const int32 _Index);
    void SetEndDataOffset(const int32 _Index);
    void AddChild(const int32 _Index);

    // the node is not guarantee to have a valid next tree level node
    UE_NODISCARD TOptional<uint16> GetNextTreeLevelNodeIndex() const;
    // the node is not guarantee to have a valid representation in the feature array
    UE_NODISCARD TOptional<KL::Debug::Features::Types::FeatureIndex> GetStartDataOffset() const;
    // the node is not guarantee to have a valid representation in the feature array
    UE_NODISCARD TOptional<KL::Debug::Features::Types::FeatureIndex> GetEndDataOffset() const;
    // get the index of the filter in the main filter array. This should be always valid
    // however to avoid problems handle also gracefully the case where there are bugs
    UE_NODISCARD TOptional<uint16> GetFilterIndex() const;
    UE_NODISCARD const TArray<ChildIndex>& GetChilds() const;

private:
    // this represents, in the tree, the next node location at the same level of this node
    uint16 mNextTreeLevelNode = TNumericLimits<uint16>::Max();
    // this represents the location of the feature in the main data array so we can retrieve it
    KL::Debug::Features::Types::FeatureIndex mStartDataOffset = TNumericLimits<KL::Debug::Features::Types::FeatureIndex>::Max();
    // this represents the location of the feature in the main data array so we can retrieve it
    KL::Debug::Features::Types::FeatureIndex mEndDataOffset = TNumericLimits<KL::Debug::Features::Types::FeatureIndex>::Max();
    // index of the filter in the main filter array
    uint16             mFilterIndex = TNumericLimits<uint16>::Max();
    TArray<ChildIndex> mChilds;
};

inline FKLDebugImGuiFilterTreeNode::FKLDebugImGuiFilterTreeNode(const uint16 _FilterIndex)
    : mFilterIndex(_FilterIndex)
{
}

inline void FKLDebugImGuiFilterTreeNode::SetNextTreeLevelNodeIndex(const int32 _Index)
{
    checkf(_Index < TNumericLimits<uint16>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    mNextTreeLevelNode = static_cast<uint16>(_Index);
}

inline void FKLDebugImGuiFilterTreeNode::SetStartDataOffset(const int32 _Index)
{
    checkf(_Index < TNumericLimits<KL::Debug::Features::Types::FeatureIndex>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    mStartDataOffset = static_cast<KL::Debug::Features::Types::FeatureIndex>(_Index);
    mEndDataOffset   = mStartDataOffset + 1;
}

inline void FKLDebugImGuiFilterTreeNode::SetEndDataOffset(const int32 _Index)
{
    checkf(_Index < TNumericLimits<KL::Debug::Features::Types::FeatureIndex>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    mEndDataOffset = static_cast<KL::Debug::Features::Types::FeatureIndex>(_Index);
}

inline void FKLDebugImGuiFilterTreeNode::AddChild(const int32 _Index)
{
    checkf(_Index < TNumericLimits<ChildIndex>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    mChilds.Emplace(static_cast<ChildIndex>(_Index));
}

inline TOptional<uint16> FKLDebugImGuiFilterTreeNode::GetNextTreeLevelNodeIndex() const
{
    if (mNextTreeLevelNode != TNumericLimits<uint16>::Max())
    {
        return TOptional<uint16>(mNextTreeLevelNode);
    }

    return TOptional<uint16>();
}

inline TOptional<KL::Debug::Features::Types::FeatureIndex> FKLDebugImGuiFilterTreeNode::GetStartDataOffset() const
{
    if (mStartDataOffset != TNumericLimits<uint16>::Max())
    {
        return TOptional<KL::Debug::Features::Types::FeatureIndex>(mStartDataOffset);
    }

    return TOptional<KL::Debug::Features::Types::FeatureIndex>();
}

inline TOptional<uint16> FKLDebugImGuiFilterTreeNode::GetFilterIndex() const
{
    if (mFilterIndex != TNumericLimits<uint16>::Max())
    {
        return TOptional<uint16>(mFilterIndex);
    }

    return TOptional<uint16>();
}

inline TOptional<KL::Debug::Features::Types::FeatureIndex> FKLDebugImGuiFilterTreeNode::GetEndDataOffset() const
{
    if (mEndDataOffset != TNumericLimits<KL::Debug::Features::Types::FeatureIndex>::Max())
    {
        return TOptional<KL::Debug::Features::Types::FeatureIndex>(mEndDataOffset);
    }

    return TOptional<KL::Debug::Features::Types::FeatureIndex>();
}

inline const TArray<FKLDebugImGuiFilterTreeNode::ChildIndex>& FKLDebugImGuiFilterTreeNode::GetChilds() const
{
    return mChilds;
}