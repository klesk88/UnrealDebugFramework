#pragma once

#include "Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"

// engine
#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerImGuiContext final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerImGuiContext(const UWorld& _World, const bool _DrawTree, const FOnImGuiFeatureStateUpdated& _FeatureUpdateDelegate, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager);

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD FKLDebugImGuiFeaturesTypesContainerManager& GetFeaturesContainerManager() const;
    UE_NODISCARD bool GetShouldDrawTree() const;
    UE_NODISCARD ENetMode GetCurrentNetMode() const;
    UE_NODISCARD const FOnImGuiFeatureStateUpdated& GetFeatureUpdateDelegate() const;

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
