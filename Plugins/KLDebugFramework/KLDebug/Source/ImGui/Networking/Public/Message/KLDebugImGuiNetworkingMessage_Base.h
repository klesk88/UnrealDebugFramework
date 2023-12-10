// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessage_Base.h"

class UObject;
class UWorld;

template <EKLDebugImGuiNetworkMessage MessageType>
class TKLDebugImGuiNetworkingMessage_Base : public TKLDebugNetworkingMessage_Base<EKLDebugImGuiNetworkMessage, MessageType, static_cast<uint16>(EKLDebugImGuiNetworkMessageTypes::ImGuiMessage)>
{
};
