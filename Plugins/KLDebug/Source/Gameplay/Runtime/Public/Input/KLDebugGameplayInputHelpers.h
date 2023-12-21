// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"

class APlayerController;
class IEnhancedInputSubsystemInterface;
class UEnhancedInputComponent;
class UInputComponent;

namespace KL::Debug::Gameplay::Input::Helpers
{
    // this method allow to bind all inputs required by the main features of the system to work on the player controller
    // so that the debug system can still work in package builds or in non editor builds
    KLDEBUGGAMEPLAYRUNTIME_API void BindMainInputsToComponent(const APlayerController& _PlayerController, UInputComponent& _OutInputComponent);
}    // namespace KL::Debug::Gameplay::Input::Helpers