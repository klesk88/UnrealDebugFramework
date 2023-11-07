#pragma once

#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

//modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessage_Base.h"

class UObject;
class UWorld;

template<EKLDebugNetworkMessageTypes MessageType>
class TKLDebugImGuiNetworkingMessage_Base : public TKLDebugNetworkingMessage_Base<EKLDebugNetworkMessageTypes, MessageType>
{
};
