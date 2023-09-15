#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerImGuiContext final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerImGuiContext(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager);

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD FKLDebugImGuiFeaturesTypesContainerManager& GetFeaturesContainerManager() const;
    UE_NODISCARD bool GetShouldDrawTree() const;

private:
    const UWorld& mWorld;
    FKLDebugImGuiFeaturesTypesContainerManager& mFeatureContainerManager;
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