// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

// engine
#include "Containers/UnrealString.h"

class FKLDebugImGuiFeatureContainerBase;
class IKLDebugImGuiFeatureInterfaceBase;
class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiFeatureVisualizerImGuiContext;
class FKLDebugImGuiFeatureVisualizerRenderContext;
class FKLDebugImGuiFeatureVisualizerIterator;
class IKLDebugImGuiFeatureInterface_Unique;
class UCanvas;
class UFont;
class UPrimitiveComponent;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizer_Unique final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    using UniqueDelegateCallback = TFunctionRef<void(const FKLDebugImGuiFeatureVisualizerEntry& /*_FeatureData*/, FKLDebugImGuiFeatureVisualizerIterator& /*_FeatureIterator*/)>;

public:
    // FKLDebugImGuiFeatureVisualizerBase
    void Init(const FKLDebugImGuiFeatureContainerBase& _Container, const FString& _TreeName, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    void TickFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem) final;
    void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const final;
    void DrawOnCanvas(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UCanvas& _Canvas, UFont& _Font, UWorld& _World) const final;
    void GatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManagerk) final;
    // FKLDebugImGuiFeatureVisualizerBase

    void ApplyDelegateMutable(const UniqueDelegateCallback& _Delegate, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager);

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem) final;
    // FKLDebugImGuiFeatureVisualizerBase

private:
    FString mTreeName;
};
