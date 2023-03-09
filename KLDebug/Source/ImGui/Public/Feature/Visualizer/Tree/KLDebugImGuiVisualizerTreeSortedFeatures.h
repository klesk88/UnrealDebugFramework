#pragma once

#include "Feature/Container/KLDebugImGuiFeatureData.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class IKLDebugImGuiFeatureInterface_SelectableObject;

class KLDEBUGIMGUI_API FKLDebugImGuiVisualizerTreeSortedFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiVisualizerTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex, const FKLDebugImGuiFeatureData& _FeatureData, const FName& _FeatureImGuiPath);

    UE_NODISCARD const FKLDebugImGuiFeatureData& GetFeatureData() const;
    UE_NODISCARD const FName&                    GetImGuiPath() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndex() const;

private:
    const FName&                                    mFeatureImGuiTreePath;
    const FKLDebugImGuiFeatureData&                 mFeatureData;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureDataIndex = 0;
};

inline FKLDebugImGuiVisualizerTreeSortedFeatures::FKLDebugImGuiVisualizerTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex, const FKLDebugImGuiFeatureData& _FeatureData, const FName& _FeatureImGuiPath)
    : mFeatureImGuiTreePath(_FeatureImGuiPath)
    , mFeatureData(_FeatureData)
    , mFeatureDataIndex(_FeatureDataIndex)
{
}

inline const FKLDebugImGuiFeatureData& FKLDebugImGuiVisualizerTreeSortedFeatures::GetFeatureData() const
{
    return mFeatureData;
}

inline const FName& FKLDebugImGuiVisualizerTreeSortedFeatures::GetImGuiPath() const
{
    return mFeatureImGuiTreePath;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiVisualizerTreeSortedFeatures::GetFeatureDataIndex() const
{
    return mFeatureDataIndex;
}