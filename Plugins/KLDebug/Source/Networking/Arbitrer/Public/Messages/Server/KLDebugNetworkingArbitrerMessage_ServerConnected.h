#pragma once

#include "Messages/KLDebugNetworkingArbitrerMessageBase.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGARBITRER_API FKLDebugNetworkingArbitrerMessage_ServerConnected final : public FKLDebugNetworkingArbitrerMessageBase<EKLDebugArbitrerMessage::ServerRegistration>
{
public:
    explicit FKLDebugNetworkingArbitrerMessage_ServerConnected(const uint32 _WorldServerPort, const uint32 _DebugPort);
    explicit FKLDebugNetworkingArbitrerMessage_ServerConnected(FArchive& _Archive);

    [[nodiscard]] uint32 Arbitrer_GetWorldServerPort() const;
    [[nodiscard]] uint32 Arbitrer_GetDebugPort() const;
    [[nodiscard]] uint32 Arbitrer_GetHeaderVersion() const;

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface

private:
    uint32 mWorldServerPort = 0;
    uint32 mDebugPort = 0;
    uint8 mHeaderVersion = 0;
};

inline uint32 FKLDebugNetworkingArbitrerMessage_ServerConnected::Arbitrer_GetWorldServerPort() const
{
    return mWorldServerPort;
}

inline uint32 FKLDebugNetworkingArbitrerMessage_ServerConnected::Arbitrer_GetDebugPort() const
{
    return mDebugPort;
}

inline uint32 FKLDebugNetworkingArbitrerMessage_ServerConnected::Arbitrer_GetHeaderVersion() const
{
    return mHeaderVersion;
}