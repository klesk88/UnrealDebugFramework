#pragma once

// engine
#include "HAL/Platform.h"

enum class KLDEBUGUSERFRAMEWORK_API EKLDebugNetworkReceiveMessageType : uint8
{
    ServerTick = 0,
    RequestUpdate
};
