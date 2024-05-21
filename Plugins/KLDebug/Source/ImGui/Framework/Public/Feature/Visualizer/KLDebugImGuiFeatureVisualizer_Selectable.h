// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

// engine
#include "Components/PrimitiveComponent.h"
#include "HAL/Platform.h"
#include "UObject/Object.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiFeatureVisualizerEntry;
class FKLDebugImGuiFeatureVisualizerImGuiContext;
class FKLDebugImGuiFeatureVisualizerIterator;
class FKLDebugImGuiFeatureVisualizerRenderContext;
class IKLDebugFeatureInterfaceBase;
class IKLDebugFeatureInterface_Selectable;
class UCanvas;
class UFont;
class UMaterialInterface;
class UMeshComponent;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizer_Selectable final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    using SelectableDelegateCallback = TFunctionRef<void(const FKLDebugImGuiFeatureVisualizerEntry& /*_FeatureData*/, FKLDebugImGuiFeatureVisualizerIterator& /*_FeatureIterator*/, UObject& /*_Owner*/)>;

public:
    explicit FKLDebugImGuiFeatureVisualizer_Selectable(const FKLDebugImGuiFeatureContainerBase& _Container, UMaterialInterface* _MaterialInterface, UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    ~FKLDebugImGuiFeatureVisualizer_Selectable();
    [[nodiscard]] bool operator==(const UObject& _Object) const;

    // FKLDebugImGuiFeatureVisualizerBase
    [[nodiscard]] bool IsValid() const final;
    void TickFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem) final;
    void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const final;
    void DrawOnCanvas(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UCanvas& _Canvas, UFont& _Font, UWorld& _World) const final;
    void GatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) final;
    // FKLDebugImGuiFeatureVisualizerBase

    [[nodiscard]] bool ShouldKeepAlive() const;
    [[nodiscard]] const FObjectKey& GetObjectKey() const;

    void ApplyDelegateMutable(const SelectableDelegateCallback& _Delegate, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager);

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem) final;
    // FKLDebugImGuiFeatureVisualizerBase

    [[nodiscard]] UMeshComponent* TryGetMeshComponent() const;
    void SetMaterialOverlay(UMaterialInterface* _MaterialInterface);

private:
    TWeakObjectPtr<UObject> mObject;
    TWeakObjectPtr<UMaterialInterface> mOriginalMaterialOverlay;
    FObjectKey mObjectKey;
    bool mKeepAlive = true;
};

inline bool FKLDebugImGuiFeatureVisualizer_Selectable::operator==(const UObject& _Object) const
{
    return mObject.Get() == &_Object;
}

inline bool FKLDebugImGuiFeatureVisualizer_Selectable::IsValid() const
{
    return mObject.IsValid();
}

inline bool FKLDebugImGuiFeatureVisualizer_Selectable::ShouldKeepAlive() const
{
    return mKeepAlive;
}

inline const FObjectKey& FKLDebugImGuiFeatureVisualizer_Selectable::GetObjectKey() const
{
    return mObjectKey;
}
