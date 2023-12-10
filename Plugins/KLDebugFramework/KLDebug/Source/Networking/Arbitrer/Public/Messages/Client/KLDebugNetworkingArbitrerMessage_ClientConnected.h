#pragma once

#include "Messages/KLDebugNetworkingArbitrerMessageBase.h"

// engine
#include "Containers/UnrealString.h"
#include "Math/NumericLimits.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGARBITRER_API FKLDebugNetworkingArbitrerMessage_ClientConnected final : public FKLDebugNetworkingArbitrerMessageBase<EKLDebugArbitrerMessage::ClientRegistration>
{
public:
    explicit FKLDebugNetworkingArbitrerMessage_ClientConnected(const FString& _Host, const uint32 _WorldServerPort, const uint32 _ArbitrerReplyPort, const uint16 _ID);
    explicit FKLDebugNetworkingArbitrerMessage_ClientConnected(FArchive& _Archive);

    UE_NODISCARD const FString& Arbitrer_GetHost() const;
    UE_NODISCARD uint32 Arbitrer_GetWorldServerPort() const;
    UE_NODISCARD uint32 Arbitrer_GetArbitrerReplyPort() const;
    UE_NODISCARD uint16 Arbitrer_GetArbitrerClientID() const;
    UE_NODISCARD uint8 Arbitrer_GetHeaderVersion() const;

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface

private:
    FString mHost;
    uint32 mWorldServerPort = 0;
    uint32 mArbitrerReplyPort = 0;
    uint16 mClientID = 0;
    uint8 mHeaderVersion = TNumericLimits<uint32>::Max();
};

inline const FString& FKLDebugNetworkingArbitrerMessage_ClientConnected::Arbitrer_GetHost() const
{
    return mHost;
}

inline uint32 FKLDebugNetworkingArbitrerMessage_ClientConnected::Arbitrer_GetWorldServerPort() const
{
    return mWorldServerPort;
}

inline uint32 FKLDebugNetworkingArbitrerMessage_ClientConnected::Arbitrer_GetArbitrerReplyPort() const
{
    return mArbitrerReplyPort;
}

inline uint16 FKLDebugNetworkingArbitrerMessage_ClientConnected::Arbitrer_GetArbitrerClientID() const
{
    return mClientID;
}

inline uint8 FKLDebugNetworkingArbitrerMessage_ClientConnected::Arbitrer_GetHeaderVersion() const
{
    return mHeaderVersion;
}