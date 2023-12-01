// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"

// engine
#include "Engine/World.h"

FKLDebugImGuiFeatureVisualizerImGuiContext::FKLDebugImGuiFeatureVisualizerImGuiContext(const UWorld& _World, const bool _DrawTree, const FOnImGuiFeatureStateUpdated& _FeatureUpdateDelegate, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
    : mWorld(_World)
    , mFeatureContainerManager(_FeatureContainerManager)
    , mOnFeatureStateUpdatedDelegate(_FeatureUpdateDelegate)
    , mWorldNetMode(_World.GetNetMode())
    , mDrawTree(_DrawTree)
{
}
