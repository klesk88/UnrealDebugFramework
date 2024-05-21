// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"

// engine
#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerImGuiContext final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerImGuiContext(const UWorld& _World, const bool _DrawTree, const FOnImGuiFeatureStateUpdated& _FeatureUpdateDelegate, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager);

    [[nodiscard]] const UWorld& GetWorld() const;
    [[nodiscard]] FKLDebugImGuiFeaturesTypesContainerManager& GetFeaturesContainerManager() const;
    [[nodiscard]] bool GetShouldDrawTree() const;
    [[nodiscard]] ENetMode GetCurrentNetMode() const;
    [[nodiscard]] const FOnImGuiFeatureStateUpdated& GetFeatureUpdateDelegate() const;

private:
    const UWorld& mWorld;
    FKLDebugImGuiFeaturesTypesContainerManager& mFeatureContainerManager;
    const FOnImGuiFeatureStateUpdated& mOnFeatureStateUpdatedDelegate;
    ENetMode mWorldNetMode = ENetMode::NM_MAX;
    bool mDrawTree = false;
};

inline const UWorld& FKLDebugImGuiFeatureVisualizerImGuiContext::GetWorld() const
{
    return mWorld;
}

inline FKLDebugImGuiFeaturesTypesContainerManager& FKLDebugImGuiFeatureVisualizerImGuiContext::GetFeaturesContainerManager() const
{
    return mFeatureContainerManager;
}

inline bool FKLDebugImGuiFeatureVisualizerImGuiContext::GetShouldDrawTree() const
{
    return mDrawTree;
}

inline const FOnImGuiFeatureStateUpdated& FKLDebugImGuiFeatureVisualizerImGuiContext::GetFeatureUpdateDelegate() const
{
    return mOnFeatureStateUpdatedDelegate;
}

inline ENetMode FKLDebugImGuiFeatureVisualizerImGuiContext::GetCurrentNetMode() const
{
    return mWorld.GetNetMode();
}
