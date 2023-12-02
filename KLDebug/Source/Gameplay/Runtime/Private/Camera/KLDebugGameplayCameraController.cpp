// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Camera/KLDebugGameplayCameraController.h"

#include "Input/KLDebugGameplayInputHelpers.h"

// engine
#include "Components/InputComponent.h"
#include "HAL/IConsoleManager.h"

void AKLDebugGameplayCameraController::BeginPlay()
{
    Super::BeginPlay();
}

void AKLDebugGameplayCameraController::SetupInputComponent()
{
    Super::SetupInputComponent();

    KL::Debug::Gameplay::Input::Helpers::BindMainInputsToComponent(*this, *InputComponent);

    // disable blur as when teh game is pause n editor it is unusable the camera otherwise
    ConsoleCommand(TEXT("r.DefaultFeature.MotionBlur 0"));
}
