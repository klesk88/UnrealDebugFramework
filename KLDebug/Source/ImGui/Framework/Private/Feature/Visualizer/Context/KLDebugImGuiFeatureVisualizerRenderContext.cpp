#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"

//engine
#include "Engine/World.h"

FKLDebugImGuiFeatureVisualizerRenderContext::FKLDebugImGuiFeatureVisualizerRenderContext(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
    : mWorld(_World)
    , mFeatureContainerManager(_FeatureContainerManager)
{
}
