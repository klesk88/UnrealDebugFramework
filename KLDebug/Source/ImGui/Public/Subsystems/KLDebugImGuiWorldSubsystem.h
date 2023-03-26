#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer.h"
#include "Subsystems/KLDebugImGuiSubsystemUpdatable.h"

// debug utils module
#include "Utils/Public/Picker/KLDebugUtilsPicker.h"

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
    void PostInitialize() final;
    void Deinitialize() final;
    // UWorldSubsystem

    // IKLDebugImGuiSubsystemUpdatable
    void Initialize(FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) final;
    void Update(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) final;
    void DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) final;
    // IKLDebugImGuiSubsystemUpdatable

    UE_NODISCARD static UKLDebugImGuiWorldSubsystem* TryGetMutable(const UObject& _Object);

    // this can be called externally when an object is selected. For example from the editor module of the imgui
    // to support editor selection
    void OnObjectSelected(UObject& _Object);

private:
    void DrawImGuiWorld(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const;
    void UpdateSelectedObjectsVisualizers(FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);

private:
    FKLDebugUtilsPicker                        mPicker;
    TUniquePtr<FKLDebugImGuiFeatureVisualizer> mWorldVisualizer;
    TArray<FKLDebugImGuiFeatureVisualizer>     mSelectedObjectsVisualizers;
};
