#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

class FKLDebugImGuiFeatureContainerBase;
class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerSubsystem final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    explicit FKLDebugImGuiFeatureVisualizerSubsystem(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);

    // FKLDebugImGuiFeatureVisualizerBase
    void Render(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const final;
    // FKLDebugImGuiFeatureVisualizerBase

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const UWorld& _World) final;
    void DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) final;
    // FKLDebugImGuiFeatureVisualizerBase
};
