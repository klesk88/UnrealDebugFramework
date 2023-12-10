#pragma once

#include "Messages/KLDebugNetworkingArbitrerMessage_Types.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessage_Base.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageDefinitions.h"

template <EKLDebugArbitrerMessage MessageType>
class KLDEBUGNETWORKINGARBITRER_API FKLDebugNetworkingArbitrerMessageBase : public TKLDebugNetworkingMessage_Base<EKLDebugArbitrerMessage, MessageType, static_cast<uint16>(EKLDebugNetworkingMessagesTypes::Arbitrer)>
{
};
