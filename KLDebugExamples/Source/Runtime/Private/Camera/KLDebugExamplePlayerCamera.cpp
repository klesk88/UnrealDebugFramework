#include "Camera/KLDebugExamplePlayerCamera.h"

//KLDebugGameplay module
#include "Gameplay/Public/Input/KLDebugGameplayInputHelpers.h"

//engine
#include "Components/InputComponent.h"

#if WITH_KL_DEBUGGER

#endif

void AKLDebugExamplePlayerCamera::SetupInputComponent()
{
    Super::SetupInputComponent();

#if WITH_KL_DEBUGGER
    KL::Debug::Gameplay::Input::Helpers::BindMainInputsToComponent(*InputComponent);
#endif
}
