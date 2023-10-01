#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Input/KLDebugImGuiInputManager.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/ObjectPtr.h"

#include "KLDebugImGuiEngineSubsystem.generated.h"

class UKLDebugImGuiWorldSubsystem;
class UMaterialInterface;
class UObject;
class UWorld;

UCLASS(Transient)
class KLDEBUGIMGUIFRAMEWORK_API UKLDebugImGuiEngineSubsystem final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

    UE_NODISCARD static UKLDebugImGuiEngineSubsystem*              GetMutable();
    UE_NODISCARD static const UKLDebugImGuiEngineSubsystem*        Get();
    UE_NODISCARD const FKLDebugImGuiFeaturesTypesContainerManager& GetFeatureContainerManager() const;
    UE_NODISCARD FKLDebugImGuiFeaturesTypesContainerManager&       GetContainerManagerMutable();

    void              ToogleImGuiSystemState();
    UE_NODISCARD bool IsImGuiSystemEnabled() const;

    UE_NODISCARD UMaterialInterface* GetOverlayMaterial() const;

private:
    void InitFromConfig();

    void RegisterCallbacks();
    void UnreagisterCallbacks();

    void Update(const UWorld& _World);

private:
    UPROPERTY()
    TObjectPtr<UMaterialInterface> OverlayMaterial;

    FKLDebugImGuiFeaturesTypesContainerManager mFeatureContainersManger;
    FKLDebugImGuiInputManager                  mInputManager;
};

inline const UKLDebugImGuiEngineSubsystem* UKLDebugImGuiEngineSubsystem::Get()
{
    return GetMutable();
}

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