// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"

enum class KLDEBUGIMGUINETWORKING_API EKLDebugNetworkMessageTypes : uint8
{
    Client_FeatureStatusUpdate = 0,

    Server_InitClientConnection,
    Server_FeatureDataUpdate,

    Count
};

namespace KL::Debug::ImGui::Networking::Message
{
    typedef uint16 MessageID;

    static constexpr const uint32 MessageMark = 0xB50FB50F;

    KLDEBUGIMGUINETWORKING_API UE_NODISCARD MessageID GetNewMessageID();
}    // namespace KL::Debug::ImGui::Networking::Message