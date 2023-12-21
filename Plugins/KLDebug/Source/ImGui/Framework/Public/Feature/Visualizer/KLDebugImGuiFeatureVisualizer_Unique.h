// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

// engine
#include "Containers/UnrealString.h"

class FKLDebugImGuiFeatureContainerBase;
class FKLDebugImGuiFeatureVisualizerImGuiContext;
class FKLDebugImGuiFeatureVisualizerRenderContext;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizer_Unique final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    // FKLDebugImGuiFeatureVisualizerBase
    void Init(const FKLDebugImGuiFeatureContainerBase& _Container, const FString& _TreeName, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const final;
    // FKLDebugImGuiFeatureVisualizerBase

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    // FKLDebugImGuiFeatureVisualizerBase

private:
    FString mTreeName;
};
