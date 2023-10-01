#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

//engine
#include "Containers/UnrealString.h"

class FKLDebugImGuiFeatureContainerBase;
class FKLDebugImGuiFeatureVisualizerImGuiContext;
class FKLDebugImGuiFeatureVisualizerRenderContext;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerSubsystem final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    explicit FKLDebugImGuiFeatureVisualizerSubsystem(const FKLDebugImGuiFeatureContainerBase& _Container, const FString& _TreeName, const EContainerType _ContainerType, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);

    // FKLDebugImGuiFeatureVisualizerBase
    void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const final;
    // FKLDebugImGuiFeatureVisualizerBase

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    // FKLDebugImGuiFeatureVisualizerBase

private:
    FString mTreeName;
    EContainerType mContainerType = EContainerType::COUNT;
};
