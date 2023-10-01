#include "Camera/KLDebugExamplePlayerCamera.h"

//modules
#include "Gameplay/Runtime/Public/Input/KLDebugGameplayInputHelpers.h"

// engine
#include "Components/InputComponent.h"

void AKLDebugExamplePlayerCamera::SetupInputComponent()
{
    Super::SetupInputComponent();

#if WITH_KL_DEBUGGER
    KL::Debug::Gameplay::Input::Helpers::BindMainInputsToComponent(*this, *InputComponent);
#endif
}
