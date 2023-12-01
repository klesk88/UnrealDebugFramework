// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

// engine
#include "UObject/Object.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiFeatureVisualizerImGuiContext;
class FKLDebugImGuiFeatureVisualizerRenderContext;
class UMaterialInterface;
class UMeshComponent;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizer_Selectable final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    explicit FKLDebugImGuiFeatureVisualizer_Selectable(const FKLDebugImGuiFeatureContainerBase& _Container, UMaterialInterface* _MaterialInterface, UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    ~FKLDebugImGuiFeatureVisualizer_Selectable();
    UE_NODISCARD bool operator==(const UObject& _Object) const;

    // FKLDebugImGuiFeatureVisualizerBase
    UE_NODISCARD bool IsValid() const final;
    void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const final;
    // FKLDebugImGuiFeatureVisualizerBase

    UE_NODISCARD bool ShouldKeepAlive() const;
    UE_NODISCARD const FObjectKey& GetObjectKey() const;

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) final;
    // FKLDebugImGuiFeatureVisualizerBase

    UE_NODISCARD UMeshComponent* TryGetMeshComponent() const;
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
