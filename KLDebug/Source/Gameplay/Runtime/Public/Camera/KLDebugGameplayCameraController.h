#pragma once

//engine
#include "CoreMinimal.h"
#include "Engine/DebugCameraController.h"

#include "KLDebugGameplayCameraController.generated.h"

UCLASS()
class KLDEBUGGAMEPLAYRUNTIME_API AKLDebugGameplayCameraController : public ADebugCameraController
{
    GENERATED_BODY()

protected:
    //ADebugCameraController
    void BeginPlay() override;
    void SetupInputComponent() override;
    //ADebugCameraController
};