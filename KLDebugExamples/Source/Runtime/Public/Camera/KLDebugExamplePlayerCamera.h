#pragma once

// engine
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "KLDebugExamplePlayerCamera.generated.h"

UCLASS()
class KLDEBUGEXAMPLESRUNTIME_API AKLDebugExamplePlayerCamera final : public APlayerController
{
    GENERATED_BODY()

protected:
    void SetupInputComponent() final;
};