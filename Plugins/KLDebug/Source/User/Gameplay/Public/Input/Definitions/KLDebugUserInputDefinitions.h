// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "HAL/Platform.h"
#include "UObject/Class.h"

#include "KLDebugUserInputDefinitions.generated.h"

UENUM()
enum class EKLDebugGameplayInputType : uint8
{
    ToogleDebug = 0,
    ToogleInput,
    ToogleCamera,
    TooglePause,

    Count
};
