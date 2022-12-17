#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainer.h"

//utils module
#include "Utils/Public/Picker/KLDebugUtilsPicker.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
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

private:
    FKLDebugImGuiFeatureContainer mFeaturesContainer;
    FKLDebugUtilsPicker           mPicker;
};

inline const FKLDebugUtilsPicker& UKLDebugImGuiEngineSubsystem::GetPicker() const
{
    return mPicker;
}

inline FKLDebugUtilsPicker& UKLDebugImGuiEngineSubsystem::GetPickerMutable()
{
    return mPicker;
}