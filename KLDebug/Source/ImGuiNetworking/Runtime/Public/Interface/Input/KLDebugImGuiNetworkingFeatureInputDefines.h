#pragma once

// engine
#include "CoreMinimal.h"
#include "HAL/Platform.h"

enum class EKLDebugImGuiNetworkingEnviroment : uint8
{
    Server = 0,
    Client,

    Count
};