#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"

//engine
#include "Engine/World.h"

FKLDebugImGuiFeatureVisualizerImGuiContext::FKLDebugImGuiFeatureVisualizerImGuiContext(const UWorld& _World, const bool _DrawTree, const FOnImGuiFeatureStateUpdated& _FeatureUpdateDelegate, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
    : mWorld(_World)
    , mFeatureContainerManager(_FeatureContainerManager)
    , mOnFeatureStateUpdatedDelegate(_FeatureUpdateDelegate)
    , mDrawTree(_DrawTree)
{
}

ENetMode FKLDebugImGuiFeatureVisualizerImGuiContext::GetCurrentNetMode() const
{
    return mWorld.GetNetMode();
}
