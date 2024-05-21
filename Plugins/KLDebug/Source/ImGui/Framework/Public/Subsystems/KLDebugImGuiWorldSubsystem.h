// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Selectable.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"
#include "Rendering/KLDebugFrameworkRenderingDefinitions.h"
#include "Window/KLDebugImGuiWindow.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Delegates/IDelegateInstance.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "HAL/Platform.h"
#include "Subsystems/WorldSubsystem.h"
#include "Templates/UniquePtr.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"

#if DO_ENSURE
// engine
#include "HAL/CriticalSection.h"
#endif

#include "KLDebugImGuiWorldSubsystem.generated.h"

class APlayerController;
class FDebugRenderSceneProxy;
class FKLDebugFrameworkBottomBarManager;
class FKLDebugFrameworkModeManager;
class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiGatherFeatureInput;
class IKLDebugFeatureInterfaceBase;
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

    [[nodiscard]] static UKLDebugImGuiWorldSubsystem* TryGetMutable(const UObject& _Object);
    [[nodiscard]] static const UKLDebugImGuiWorldSubsystem* TryGet(const UObject& _Object);

    void Tick(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, FKLDebugFrameworkBottomBarManager& _BottomBarManager, FKLDebugFrameworkModeManager& _ModeManager);
    void DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, FKLDebugFrameworkBottomBarManager& _BottomBarManager, FKLDebugFrameworkModeManager& _ModeManager);
    void Render(const UWorld& _CurrentWorldUpdated, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const;

    // this can be called externally when an object is selected. For example from the editor module of the imgui
    // to support editor selection
    void OnObjectSelected(UObject& _Object);

    [[nodiscard]] FDelegateHandle BindOnImGuiFeatureStateUpdated(const FOnImGuiFeatureStateUpdated::FDelegate& _Delegate);
    void UnbindOnImGuiFeatureStateUpdated(FDelegateHandle& _Handle);
    [[nodiscard]] FDelegateHandle BindOnFeatureRequestUpdate(const FOnFeaturesTick::FDelegate& _Delegate);
    void UnbindOnFeatureRequestUpdate(FDelegateHandle& _Handle);
    [[nodiscard]] FDelegateHandle BindOnWindowsStatusChange(const FOnWindowStatusChange::FDelegate& _Delegate);
    void UnbindOnWindowsStatusChange(FDelegateHandle& _Handle);
    [[nodiscard]] FDelegateHandle BindOnShouldTickDelegate(const FOnShouldTickDelegate::FDelegate& _Delegate);
    void UnbindOnShouldTickDelegate(FDelegateHandle& _Handle);

    void TryGatherFeatureAndContext(FKLDebugImGuiGatherFeatureInput& _Input) const;
    void TryGatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback);

    [[nodiscard]] bool GetShouldTick() const;

    [[nodiscard]] IKLDebugContextInterface* GetCurrentWindowData(const EKLDebugWindowTypes _WindowType, int32& _Index) const;

private:
    void DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem);
    void DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem);
    void DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem);

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
    FKLDebugImGuiWindow mImGuiWindow;
    FDelegateHandle mImGuiCanvasDelegateHandle;
    FDelegateHandle mOnFeatureUpdateDelegateHandle;
    FOnImGuiFeatureStateUpdated mOnFeaturesUpdatedDelegate;
    FOnWindowStatusChange mOnWindowStatusChange;
    FOnFeaturesTick mOnFeaturesTick;
    FOnShouldTickDelegate mOnShouldTickDelegate;
    bool mShouldStoreDelta = false;
    bool mShouldTick = false;
    bool mRenderModeWindow = true;

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

inline FDelegateHandle UKLDebugImGuiWorldSubsystem::BindOnFeatureRequestUpdate(const FOnFeaturesTick::FDelegate& _Delegate)
{
    return mOnFeaturesTick.Add(_Delegate);
}

inline void UKLDebugImGuiWorldSubsystem::UnbindOnFeatureRequestUpdate(FDelegateHandle& _Handle)
{
    mOnFeaturesTick.Remove(_Handle);
    _Handle.Reset();
}

inline FDelegateHandle UKLDebugImGuiWorldSubsystem::BindOnWindowsStatusChange(const FOnWindowStatusChange::FDelegate& _Delegate)
{
    return mOnWindowStatusChange.Add(_Delegate);
}

inline void UKLDebugImGuiWorldSubsystem::UnbindOnWindowsStatusChange(FDelegateHandle& _Handle)
{
    mOnWindowStatusChange.Remove(_Handle);
    _Handle.Reset();
}

inline FDelegateHandle UKLDebugImGuiWorldSubsystem::BindOnShouldTickDelegate(const FOnShouldTickDelegate::FDelegate& _Delegate)
{
    return mOnShouldTickDelegate.Add(_Delegate);
}

inline void UKLDebugImGuiWorldSubsystem::UnbindOnShouldTickDelegate(FDelegateHandle& _Handle)
{
    mOnShouldTickDelegate.Remove(_Handle);
    _Handle.Reset();
}

inline bool UKLDebugImGuiWorldSubsystem::GetShouldTick() const
{
    return mShouldTick;
}