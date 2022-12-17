#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainer.h"

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
    UE_NODISCARD static UKLDebugImGuiEngineSubsystem* Get();

    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

private:
    FKLDebugImGuiFeatureContainer mFeaturesContainer;
};
