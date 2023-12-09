// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugNetworkingMessageInterface.h"

template <typename MessageEnumType, MessageEnumType EnumValue, uint16 EnumTypeIndex>
class TKLDebugNetworkingMessage_Base : public IKLDebugNetworkingMessageInterface
{
public:
    // IKLDebugNetworkingMessageInterface
    UE_NODISCARD uint16 GetMessageType() const final;
    UE_NODISCARD uint16 GetMessageEnumType() const final;
    // IKLDebugNetworkingMessageInterface
};

template <typename MessageEnumType, MessageEnumType EnumValue, uint16 EnumTypeIndex>
inline uint16 TKLDebugNetworkingMessage_Base<MessageEnumType, EnumValue, EnumTypeIndex>::GetMessageType() const
{
    return static_cast<uint16>(EnumValue);
}

template <typename MessageEnumType, MessageEnumType EnumValue, uint16 EnumTypeIndex>
inline uint16 TKLDebugNetworkingMessage_Base<MessageEnumType, EnumValue, EnumTypeIndex>::GetMessageEnumType() const
{
    return EnumTypeIndex;
}