#pragma once

//engine
#include "CoreMinimal.h"

class UInputComponent;

namespace KL::Debug::Gameplay::Input::Helpers
{
    KLDEBUGGAMEPLAY_API void BindMainInputsToComponent(UInputComponent& _OutInputComponent);
}