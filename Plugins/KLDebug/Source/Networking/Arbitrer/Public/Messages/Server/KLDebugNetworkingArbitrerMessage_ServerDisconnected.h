#pragma once

#include "Messages/KLDebugNetworkingArbitrerMessageBase.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGARBITRER_API FKLDebugNetworkingArbitrerMessage_ServerDisconnected final : public FKLDebugNetworkingArbitrerMessageBase<EKLDebugArbitrerMessage::ServerDisconnect>
{
public:
    explicit FKLDebugNetworkingArbitrerMessage_ServerDisconnected(const uint32 _WorldServerPort);
    explicit FKLDebugNetworkingArbitrerMessage_ServerDisconnected(FArchive& _Archive);

    UE_NODISCARD uint32 Arbitrer_GetWorldServerPort() const;

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface

private:
    uint32 mWorldServerPort = 0;
};

inline uint32 FKLDebugNetworkingArbitrerMessage_ServerDisconnected::Arbitrer_GetWorldServerPort() const
{
    return mWorldServerPort;
}
