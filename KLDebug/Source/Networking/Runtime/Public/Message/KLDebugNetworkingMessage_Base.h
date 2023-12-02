// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugNetworkingMessageInterface.h"

template <typename MessageEnumType, MessageEnumType EnumValue>
class TKLDebugNetworkingMessage_Base : public IKLDebugNetworkingMessageInterface
{
public:
    // IKLDebugNetworkingMessageInterface
    UE_NODISCARD uint16 GetMessageType() const final;
    // IKLDebugNetworkingMessageInterface
};

template <typename MessageEnumType, MessageEnumType EnumValue>
inline uint16 TKLDebugNetworkingMessage_Base<MessageEnumType, EnumValue>::GetMessageType() const
{
    return static_cast<uint16>(EnumValue);
}
