// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Messages/KLDebugNetworkingArbitrerMessageBase.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGARBITRER_API FKLDebugNetworkingArbitrerMessage_ServerPing final : public FKLDebugNetworkingArbitrerMessageBase<EKLDebugArbitrerMessage::ServerPing>
{
public:
    explicit FKLDebugNetworkingArbitrerMessage_ServerPing(const uint32 _AnswerPort);
    explicit FKLDebugNetworkingArbitrerMessage_ServerPing(FArchive& _Archive);

    UE_NODISCARD uint32 Arbitrer_GetAnswerPort() const;

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface

private:
    uint32 mAnswerPort = 0;
};

inline uint32 FKLDebugNetworkingArbitrerMessage_ServerPing::Arbitrer_GetAnswerPort() const
{
    return mAnswerPort;
}
