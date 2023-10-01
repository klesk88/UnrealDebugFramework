#pragma once

#include "Feature/Container/KLDebugImGuiFeatureData.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderData.h"

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class IKLDebugImGuiFeatureInterface_SelectableObject;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiVisualizerTreeSortedFeatures final : public FKLDebugImGuiTreeBuilderData
{
public:
    explicit FKLDebugImGuiVisualizerTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex, const FKLDebugImGuiFeatureData& _FeatureData, const FName& _FeatureImGuiPath);

    // FKLDebugImGuiTreeBuilderData
    UE_NODISCARD const TArray<FName>& GetPathTokens() const final;
    // FKLDebugImGuiTreeBuilderData

    UE_NODISCARD const FKLDebugImGuiFeatureData& GetFeatureData() const;

private:
    const FKLDebugImGuiFeatureData& mFeatureData;
};

inline FKLDebugImGuiVisualizerTreeSortedFeatures::FKLDebugImGuiVisualizerTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex, const FKLDebugImGuiFeatureData& _FeatureData, const FName& _FeatureImGuiPath)
    : FKLDebugImGuiTreeBuilderData(_FeatureDataIndex, _FeatureImGuiPath)
    , mFeatureData(_FeatureData)
{
}

inline const FKLDebugImGuiFeatureData& FKLDebugImGuiVisualizerTreeSortedFeatures::GetFeatureData() const
{
    return mFeatureData;
}

inline const TArray<FName>& FKLDebugImGuiVisualizerTreeSortedFeatures::GetPathTokens() const
{
    return mFeatureData.GetImGuiPathTokens();
}