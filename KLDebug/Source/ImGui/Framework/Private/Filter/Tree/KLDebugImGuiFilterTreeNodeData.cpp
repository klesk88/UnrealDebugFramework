#include "Filter/Tree/KLDebugImGuiFilterTreeNodeData.h"

FKLDebugImGuiFilterTreeNodeData::FKLDebugImGuiFilterTreeNodeData(const int32 _FeatureIndex)
    : mStartDataOffset(static_cast<KL::Debug::ImGui::Features::Types::FeatureIndex>(_FeatureIndex))
{
    checkf(_FeatureIndex < TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max(), TEXT("index outside range. we don't expect a tree of this size"));
    checkf(_FeatureIndex + 1 < TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max(), TEXT("index outside range. we don't expect a tree of this size"));

    mEndDataOffset = mStartDataOffset + 1;
}
