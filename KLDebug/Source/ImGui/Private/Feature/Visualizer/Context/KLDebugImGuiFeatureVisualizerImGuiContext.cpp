#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"

//engine
#include "Engine/World.h"

FKLDebugImGuiFeatureVisualizerImGuiContext::FKLDebugImGuiFeatureVisualizerImGuiContext(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
    : mWorld(_World)
    , mFeatureContainerManager(_FeatureContainerManager)
    , mDrawTree(_DrawTree)
{
}
