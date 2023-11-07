#include "Camera/KLDebugGameplayCameraController.h"

#include "Input/KLDebugGameplayInputHelpers.h"

// engine
#include "Components/InputComponent.h"

void AKLDebugGameplayCameraController::BeginPlay()
{
    Super::BeginPlay();
}

void AKLDebugGameplayCameraController::SetupInputComponent()
{
    Super::SetupInputComponent();

    KL::Debug::Gameplay::Input::Helpers::BindMainInputsToComponent(*this, *InputComponent);
}
