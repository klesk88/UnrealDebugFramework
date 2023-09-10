#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSelectableObject.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSubsystem.h"
#include "Subsystems/KLDebugImGuiSubsystemUpdatable.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "Subsystems/WorldSubsystem.h"
#include "Templates/UniquePtr.h"

#include "KLDebugImGuiWorldSubsystem.generated.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class IKLDebugImGuiFeatureInterfaceBase;

UCLASS(Transient)
class KLDEBUGIMGUI_API UKLDebugImGuiWorldSubsystem final : public UWorldSubsystem, public IKLDebugImGuiSubsystemUpdatable
{
    GENERATED_BODY()

public:
    // UWorldSubsystem
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;
    // UWorldSubsystem

    // IKLDebugImGuiSubsystemUpdatable
    void Initialize(FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) final;
    void Update(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) final;
    void DrawImGui(const UWorld& _CurrentWorldUpdated, const bool _TabOpen, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) final;
    // IKLDebugImGuiSubsystemUpdatable

    UE_NODISCARD static UKLDebugImGuiWorldSubsystem* TryGetMutable(const UObject& _Object);

    // this can be called externally when an object is selected. For example from the editor module of the imgui
    // to support editor selection
    void OnObjectSelected(UObject& _Object);

private:
    void DrawImGuiWorld(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);

private:
    TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem>    mWorldVisualizer;
    TArray<FKLDebugImGuiFeatureVisualizerSelectableObject> mSelectedObjectsVisualizers;
    bool                                                   mIsInitialized = false;
};
