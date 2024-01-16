#pragma once

// engine
#include "HAL/Platform.h"

enum class KLDEBUGUSERNETWORKING_API EKLDebugNetworkReceiveMessageType : uint8
{
    ServerTick = 0,
    RequestUpdate
};
