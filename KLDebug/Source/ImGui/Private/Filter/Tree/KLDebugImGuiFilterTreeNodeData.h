#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFilterTreeNodeData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFilterTreeNodeData(const int32 _FeatureIndex);

    void SetEndDataOffset(const int32 _Index);

    // the node is not guarantee to have a valid representation in the feature array
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetStartDataOffset() const;
    // the node is not guarantee to have a valid representation in the feature array
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetEndDataOffset() const;

private:
    // this represents the location of the feature in the main data array so we can retrieve it
    KL::Debug::ImGui::Features::Types::FeatureIndex mStartDataOffset = TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max();
    // this represents the location of the feature in the main data array so we can retrieve it
    KL::Debug::ImGui::Features::Types::FeatureIndex mEndDataOffset = TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max();
};

inline void FKLDebugImGuiFilterTreeNodeData::SetEndDataOffset(const int32 _Index)
{
    checkf(_Index < TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    ensureMsgf(_Index > mStartDataOffset, TEXT("index must be higher"));

    mEndDataOffset = static_cast<KL::Debug::ImGui::Features::Types::FeatureIndex>(_Index);
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFilterTreeNodeData::GetStartDataOffset() const
{
    return mStartDataOffset;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFilterTreeNodeData::GetEndDataOffset() const
{
    return mEndDataOffset;
}
