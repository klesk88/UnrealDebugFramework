// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Selectable.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"

// engine
#include "CoreMinimal.h"
#include "Engine/World.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeaturesTickInput
{
public:
    explicit FKLDebugImGuiFeaturesTickInput(const UWorld& _World, FKLDebugImGuiFeatureVisualizer_Unique& _UniqueFeatures, TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& _ObjectFeatures, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _Flags);

    UE_NODISCARD const UWorld& GetOwnerWorld() const;
    UE_NODISCARD FKLDebugImGuiFeatureVisualizer_Unique& GetUniqueFeaturesMutable() const;
    UE_NODISCARD TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& GetObjectFeaturesMutable() const;
    UE_NODISCARD FKLDebugImGuiFeaturesTypesContainerManager& GetContainerManagerMutable() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureEnableSet& GetFeatureFlagsMutable() const;

private:
    const UWorld& mWorld;
    FKLDebugImGuiFeatureVisualizer_Unique& mUniqueFeatures;
    TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& mObjectFeatures;
    FKLDebugImGuiFeaturesTypesContainerManager& mContainerManager;
    KL::Debug::ImGui::Features::Types::FeatureEnableSet& mFlags;
};

inline const UWorld& FKLDebugImGuiFeaturesTickInput::GetOwnerWorld() const
{
    return mWorld;
}

inline FKLDebugImGuiFeatureVisualizer_Unique& FKLDebugImGuiFeaturesTickInput::GetUniqueFeaturesMutable() const
{
    return mUniqueFeatures;
}

inline TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& FKLDebugImGuiFeaturesTickInput::GetObjectFeaturesMutable() const
{
    return mObjectFeatures;
}

inline FKLDebugImGuiFeaturesTypesContainerManager& FKLDebugImGuiFeaturesTickInput::GetContainerManagerMutable() const
{
    return mContainerManager;
}

inline KL::Debug::ImGui::Features::Types::FeatureEnableSet& FKLDebugImGuiFeaturesTickInput::GetFeatureFlagsMutable() const
{
    return mFlags;
}