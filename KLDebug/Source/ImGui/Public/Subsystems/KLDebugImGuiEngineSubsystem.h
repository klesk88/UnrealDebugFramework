#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainer.h"
#include "Input/KLDebugImGuiInputManager.h"

//utils module
#include "Utils/Public/Picker/KLDebugUtilsPicker.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
 #include "Subsystems/EngineSubsystem.h"

#include "KLDebugImGuiEngineSubsystem.generated.h"

class IKLDebugImGuiFeatureInterface;

UCLASS()
class KLDEBUGIMGUI_API UKLDebugImGuiEngineSubsystem final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

    UE_NODISCARD static UKLDebugImGuiEngineSubsystem* Get();
    UE_NODISCARD const FKLDebugUtilsPicker& GetPicker() const;
    UE_NODISCARD FKLDebugUtilsPicker& GetPickerMutable();

    void ToogleImGuiSystemState();

private:
    void RegisterCallbacks();
    void UnreagisterCallbacks();

    void Update();

private:
    FKLDebugImGuiFeatureContainer mFeaturesContainer;
    FKLDebugUtilsPicker           mPicker;
    FKLDebugImGuiInputManager     mInputManager;
};

inline const FKLDebugUtilsPicker& UKLDebugImGuiEngineSubsystem::GetPicker() const
{
    return mPicker;
}

inline FKLDebugUtilsPicker& UKLDebugImGuiEngineSubsystem::GetPickerMutable()
{
    return mPicker;
}

inline void UKLDebugImGuiEngineSubsystem::ToogleImGuiSystemState()
{
    mInputManager.ToogleImGuiSystem();
}