// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageDefinitions.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"

enum class KLDEBUGIMGUINETWORKING_API EKLDebugImGuiNetworkMessageTypes : uint8
{
    ImGuiMessage = EKLDebugNetworkingMessagesTypes::Count,

    Count
};

enum class KLDEBUGIMGUINETWORKING_API EKLDebugImGuiNetworkMessage : uint8
{
    Client_FeatureStatusUpdate = 0,
    Client_FeatureRequestUpdate,
    Client_WindowToogle,
    Client_WindowRequestUpdate,

    Server_InitClientConnection,
    Server_FeatureDataUpdate,
    Server_WindowDataUpdate,

    Count
};
