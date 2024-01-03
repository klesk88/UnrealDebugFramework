// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "HAL/Platform.h"

enum class KLDEBUGNETWORKINGRUNTIME_API EKLDebugNetworkingMessagesTypes : uint8
{
    Arbitrer = 0,
    NetworkingRuntime,
    Command,

    Count
};

enum class KLDEBUGNETWORKINGRUNTIME_API EKLDebugNetworkingMessage : uint8
{
    IsConnectionAlive = 0,
    ClientConnected,
};

enum class KLDEBUGNETWORKINGRUNTIME_API EKLDebugNetworkingCommandMessage : uint8
{
    SyncCommand = 0
};
