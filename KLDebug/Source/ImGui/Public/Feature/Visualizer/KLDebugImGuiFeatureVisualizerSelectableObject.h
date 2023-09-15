#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

// engine
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class UMaterialInterface;
class UMeshComponent;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerSelectableObject final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    explicit FKLDebugImGuiFeatureVisualizerSelectableObject(const FKLDebugImGuiFeatureContainerBase& _Container, UMaterialInterface* _MaterialInterface, UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    ~FKLDebugImGuiFeatureVisualizerSelectableObject();

    UE_NODISCARD bool operator==(const UObject& _Object) const;

    // FKLDebugImGuiFeatureVisualizerBase
    UE_NODISCARD bool IsValid() const final;
    void              Render(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) const final;
    // FKLDebugImGuiFeatureVisualizerBase

    UE_NODISCARD bool ShouldKeepAlive() const;

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const UWorld& _World) final;
    void DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) final;
    // FKLDebugImGuiFeatureVisualizerBase

    UE_NODISCARD UMeshComponent* TryGetMeshComponent() const;
    void SetMaterialOverlay(UMaterialInterface* _MaterialInterface);

private:
    TWeakObjectPtr<UObject> mObject;
    TWeakObjectPtr<UMaterialInterface> mOriginalMaterialOverlay;
    bool mKeepAlive = true;
};

inline bool FKLDebugImGuiFeatureVisualizerSelectableObject::operator==(const UObject& _Object) const
{
    return mObject.Get() == &_Object;
}

inline bool FKLDebugImGuiFeatureVisualizerSelectableObject::IsValid() const
{
    return mObject.IsValid();
}

inline bool FKLDebugImGuiFeatureVisualizerSelectableObject::ShouldKeepAlive() const
{
    return mKeepAlive;
}