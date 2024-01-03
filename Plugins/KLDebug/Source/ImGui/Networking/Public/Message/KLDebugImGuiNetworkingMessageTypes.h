// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "Networking/runtime/Public/Message/KLDebugNetworkingMessageDefinitions.h"

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

    Server_InitClientConnection,
    Server_FeatureDataUpdate,

    Count
};
