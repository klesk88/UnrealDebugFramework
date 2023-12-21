// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerRenderContext final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerRenderContext(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager);

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD const FKLDebugImGuiFeaturesTypesContainerManager& GetFeaturesContainerManager() const;

private:
    const UWorld& mWorld;
    const FKLDebugImGuiFeaturesTypesContainerManager& mFeatureContainerManager;
};

inline const UWorld& FKLDebugImGuiFeatureVisualizerRenderContext::GetWorld() const
{
    return mWorld;
}

inline const FKLDebugImGuiFeaturesTypesContainerManager& FKLDebugImGuiFeatureVisualizerRenderContext::GetFeaturesContainerManager() const
{
    return mFeatureContainerManager;
}
