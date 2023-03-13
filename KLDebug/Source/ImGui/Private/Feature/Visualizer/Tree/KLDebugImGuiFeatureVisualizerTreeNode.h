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

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerTreeNode final : public FKLDebugImGuiTreeBuilderNode
{
public:
    FKLDebugImGuiFeatureVisualizerTreeNode() = default;
    explicit FKLDebugImGuiFeatureVisualizerTreeNode(const int32 _NodeDataIndex);

    void AddFeatureDataIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex);

private:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesIndexes;
};

inline void FKLDebugImGuiFeatureVisualizerTreeNode::AddFeatureDataIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex)
{
    mFeaturesIndexes.Emplace(_FeatureDataIndex);
}
