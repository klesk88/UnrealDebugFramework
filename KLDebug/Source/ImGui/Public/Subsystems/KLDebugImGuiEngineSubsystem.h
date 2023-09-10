#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Input/KLDebugImGuiInputManager.h"
#include "Window/KLDebugImGuiWindow.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "InstancedStruct.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/ObjectPtr.h"
#include "UObject/WeakInterfacePtr.h"

#include "KLDebugImGuiEngineSubsystem.generated.h"

class IKLDebugImGuiFeatureInterfaceBase;
class FKLDebugImGuiFeatureVisualizerSubsystem;
class IKLDebugImGuiSubsystemUpdatable;
class UMaterialInterface;
class UObject;
class UWorld;

UCLASS(Transient)
class KLDEBUGIMGUI_API UKLDebugImGuiEngineSubsystem final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

    UE_NODISCARD static UKLDebugImGuiEngineSubsystem*              Get();
    UE_NODISCARD const FKLDebugImGuiFeaturesTypesContainerManager& GetFeatureContainerManager() const;
    UE_NODISCARD FKLDebugImGuiFeaturesTypesContainerManager&       GetContainerManagerMutable();

    void              ToogleImGuiSystemState();
    UE_NODISCARD bool IsImGuiSystemEnabled() const;

    void AddUpdatableSystem(IKLDebugImGuiSubsystemUpdatable& _System);
    void RemoveUpdatableSystem(const IKLDebugImGuiSubsystemUpdatable& _System, const bool _IsRegistered);

    UE_NODISCARD UMaterialInterface* GetOverlayMaterial() const;

private:
    void InitFromConfig();
    void InitEngineVisualizer();

    void RegisterCallbacks();
    void UnreagisterCallbacks();

    void Update(const UWorld& _World);

    void AddPendingUpdatableSystems();
    void UpdateSystems(const UWorld& _World);
    void DrawImGui(const UWorld& _World);

private:
    UPROPERTY()
    TObjectPtr<UMaterialInterface> OverlayMaterial;

    FKLDebugImGuiFeaturesTypesContainerManager                 mFeatureContainersManger;
    FKLDebugImGuiInputManager                                  mInputManager;
    FInstancedStruct                                           mImGuiWindow;
    TArray<TWeakInterfacePtr<IKLDebugImGuiSubsystemUpdatable>> mPendingUpdatableSystems;
    TArray<TWeakInterfacePtr<IKLDebugImGuiSubsystemUpdatable>> mUpdatableSystems;
    TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem>        mEngineFeaturesVisualizer;
};

inline void UKLDebugImGuiEngineSubsystem::ToogleImGuiSystemState()
{
    mInputManager.ToogleImGuiSystem();
}

inline bool UKLDebugImGuiEngineSubsystem::IsImGuiSystemEnabled() const
{
    return mInputManager.IsEnable();
}

inline const FKLDebugImGuiFeaturesTypesContainerManager& UKLDebugImGuiEngineSubsystem::GetFeatureContainerManager() const
{
    return mFeatureContainersManger;
}

inline FKLDebugImGuiFeaturesTypesContainerManager& UKLDebugImGuiEngineSubsystem::GetContainerManagerMutable()
{
    return mFeatureContainersManger;
}