// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "GenericPlatform/GenericPlatform.h"

enum class KLDEBUGNETWORKINGRUNTIME_API EKLDebugNetworkingMessagesTypes : uint8
{
    Arbitrer = 0,
    NetworkingRuntime,

    Count
};

enum class KLDEBUGNETWORKINGRUNTIME_API EKLDebugNetworkingMessage : uint8
{
    IsConnectionAlive = 0
};
