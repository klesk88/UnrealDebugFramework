// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "BottomBar/Manager/KLDebugFrameworkBottomBarManager.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Input/KLDebugImGuiInputManager.h"
#include "Mode/Manager/KLDebugFrameworkModeManager.h"

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

    [[nodiscard]] static UKLDebugImGuiEngineSubsystem* GetMutable();
    [[nodiscard]] static const UKLDebugImGuiEngineSubsystem* Get();
    [[nodiscard]] const FKLDebugImGuiFeaturesTypesContainerManager& GetFeatureContainerManager() const;
    [[nodiscard]] FKLDebugImGuiFeaturesTypesContainerManager& GetContainerManagerMutable();
    [[nodiscard]] const FKLDebugFrameworkModeManager& GetModeManager() const;
    [[nodiscard]] FKLDebugFrameworkModeManager& GetModeManagerMutable();
    [[nodiscard]] const FKLDebugFrameworkBottomBarManager& GetBottomWindowManager() const;
    [[nodiscard]] FKLDebugFrameworkBottomBarManager& GetBottomWindowManagerMutable();

    void ToogleImGuiSystemState();
    void ToogleImGuiInput();

    [[nodiscard]] bool IsImGuiSystemEnabled() const;

    [[nodiscard]] UMaterialInterface* GetOverlayMaterial() const;

private:
    void InitFromConfig();

    void RegisterCallbacks();
    void UnreagisterCallbacks();

    void Update(const UWorld& _World);

private:
    UPROPERTY()
    TObjectPtr<UMaterialInterface> OverlayMaterial;

    FKLDebugImGuiFeaturesTypesContainerManager mFeatureContainersManger;
    FKLDebugImGuiInputManager mInputManager;
    FKLDebugFrameworkBottomBarManager mBottomBarManager;
    FKLDebugFrameworkModeManager mModeManager;
};

inline const UKLDebugImGuiEngineSubsystem* UKLDebugImGuiEngineSubsystem::Get()
{
    return GetMutable();
}

inline void UKLDebugImGuiEngineSubsystem::ToogleImGuiSystemState()
{
    mInputManager.ToogleImGuiSystem();
}

inline void UKLDebugImGuiEngineSubsystem::ToogleImGuiInput()
{
    mInputManager.ToogleImGuiInput();
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

inline const FKLDebugFrameworkModeManager& UKLDebugImGuiEngineSubsystem::GetModeManager() const
{
    return mModeManager;
}

inline FKLDebugFrameworkModeManager& UKLDebugImGuiEngineSubsystem::GetModeManagerMutable()
{
    return mModeManager;
}

inline const FKLDebugFrameworkBottomBarManager& UKLDebugImGuiEngineSubsystem::GetBottomWindowManager() const
{
    return mBottomBarManager;
}

inline FKLDebugFrameworkBottomBarManager& UKLDebugImGuiEngineSubsystem::GetBottomWindowManagerMutable()
{
    return mBottomBarManager;
}