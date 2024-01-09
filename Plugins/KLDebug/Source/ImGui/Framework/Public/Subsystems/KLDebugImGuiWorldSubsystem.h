// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Selectable.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"
#include "Rendering/KLDebugFrameworkRenderingDefinitions.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "InstancedStruct.h"
#include "Subsystems/WorldSubsystem.h"
#include "Templates/UniquePtr.h"
#include "UObject/ObjectPtr.h"

#if DO_ENSURE
// engine
#include "HAL/CriticalSection.h"
#endif

#include "KLDebugImGuiWorldSubsystem.generated.h"

class APlayerController;
class FDebugRenderSceneProxy;
class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiGatherFeatureInput;
class IKLDebugImGuiFeatureInterfaceBase;
class UCanvas;
class UKLDebugFrameworkRenderingComponent;
class UPrimitiveComponent;

UCLASS(Transient)
class KLDEBUGIMGUIFRAMEWORK_API UKLDebugImGuiWorldSubsystem final : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // UWorldSubsystem
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;
    void OnWorldBeginPlay(UWorld& _World) final;
    // UWorldSubsystem

    UE_NODISCARD static UKLDebugImGuiWorldSubsystem* TryGetMutable(const UObject& _Object);
    UE_NODISCARD static const UKLDebugImGuiWorldSubsystem* TryGet(const UObject& _Object);

    void Tick(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void Render(const UWorld& _CurrentWorldUpdated, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const;

    // this can be called externally when an object is selected. For example from the editor module of the imgui
    // to support editor selection
    void OnObjectSelected(UObject& _Object);

    FDelegateHandle BindOnImGuiFeatureStateUpdated(const FOnImGuiFeatureStateUpdated::FDelegate& _Delegate);
    void UnbindOnImGuiFeatureStateUpdated(FDelegateHandle& _Handle);

    void TryGatherFeatureAndContext(FKLDebugImGuiGatherFeatureInput& _Input) const;
    void TryGatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback);

    UE_NODISCARD bool GetShouldTick() const;

private:
    void DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);

    void RegisterCanvasCallback(const UWorld& _World);
    void UnregisterCanvasCallback(const UWorld& _World);
    void DrawOnCanvas(UCanvas* _Canvas, APlayerController* _PlayerController);

    void RegisterRenderComponent() const;
    void UnregisterRenderComponent() const;

    void UpdateCanvasAndRenderStatus();

    void OnFeatureStatusUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);

private:
    UPROPERTY(Transient)
    TObjectPtr<UKLDebugFrameworkRenderingComponent> RenderingCmp;

    FKLDebugImGuiFeatureVisualizer_Unique mUniqueFeaturesVisualizer;
    TArray<FKLDebugImGuiFeatureVisualizer_Selectable> mSelectedObjectsVisualizers;
    TArray<int32> mUpdateSystems;
    FString mImGuiTreeName;
    FInstancedStruct mImGuiWindow;
    FDelegateHandle mImGuiCanvasDelegateHandle;
    FDelegateHandle mOnFeatureUpdateDelegateHandle;
    FOnImGuiFeatureStateUpdated mOnFeaturesUpdatedDelegate;
    bool mShouldStoreDelta = false;
    bool mShouldTick = false;

#if DO_ENSURE
private:
    mutable FCriticalSection mEnsureCriticalSectionRenderer;
#endif
};

inline const UKLDebugImGuiWorldSubsystem* UKLDebugImGuiWorldSubsystem::TryGet(const UObject& _Object)
{
    return TryGetMutable(_Object);
}

inline FDelegateHandle UKLDebugImGuiWorldSubsystem::BindOnImGuiFeatureStateUpdated(const FOnImGuiFeatureStateUpdated::FDelegate& _Delegate)
{
    return mOnFeaturesUpdatedDelegate.Add(_Delegate);
}

inline void UKLDebugImGuiWorldSubsystem::UnbindOnImGuiFeatureStateUpdated(FDelegateHandle& _Handle)
{
    mOnFeaturesUpdatedDelegate.Remove(_Handle);
    _Handle.Reset();
}

inline bool UKLDebugImGuiWorldSubsystem::GetShouldTick() const
{
    return mShouldTick;
}