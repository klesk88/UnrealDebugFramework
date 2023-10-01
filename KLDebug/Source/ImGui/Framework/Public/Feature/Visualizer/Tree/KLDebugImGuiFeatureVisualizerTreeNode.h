#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderNode.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerTreeNode final : public FKLDebugImGuiTreeBuilderNode
{
public:
    FKLDebugImGuiFeatureVisualizerTreeNode() = default;
    explicit FKLDebugImGuiFeatureVisualizerTreeNode(const int32 _NodeDataIndex);

    void               AddFeatureDataIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex);
    UE_NODISCARD bool  HasFeatures() const;
    UE_NODISCARD const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& GetFeatureIndexes() const;

private:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesIndexes;
};

inline void FKLDebugImGuiFeatureVisualizerTreeNode::AddFeatureDataIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex)
{
    mFeaturesIndexes.Emplace(_FeatureDataIndex);
}

inline bool FKLDebugImGuiFeatureVisualizerTreeNode::HasFeatures() const
{
    return !mFeaturesIndexes.IsEmpty();
}

inline const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& FKLDebugImGuiFeatureVisualizerTreeNode::GetFeatureIndexes() const
{
    return mFeaturesIndexes;
}