#pragma once

#include "Messages/KLDebugNetworkingArbitrerMessageBase.h"

// engine
#include "Containers/UnrealString.h"
#include "Math/NumericLimits.h"
#include "Misc/NetworkGuid.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGARBITRER_API FKLDebugNetworkingArbitrerMessage_ClientServerData final : public FKLDebugNetworkingArbitrerMessageBase<EKLDebugArbitrerMessage::Client_GetServerData>
{
public:
    explicit FKLDebugNetworkingArbitrerMessage_ClientServerData(const uint32 _DebugPort, const uint16 _ClientID, const bool _CanConnect);
    explicit FKLDebugNetworkingArbitrerMessage_ClientServerData(FArchive& _Archive);

    [[nodiscard]] uint32 Client_GetDebugPort() const;
    [[nodiscard]] uint16 Client_GetClientID() const;
    [[nodiscard]] bool Client_GetCanConnect() const;

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface

private:
    uint32 mDebugPort = 0;
    uint16 mClientID = 0;
    bool mCanConnect = false;
};

inline uint32 FKLDebugNetworkingArbitrerMessage_ClientServerData::Client_GetDebugPort() const
{
    return mDebugPort;
}

inline uint16 FKLDebugNetworkingArbitrerMessage_ClientServerData::Client_GetClientID() const
{
    return mClientID;
}

inline bool FKLDebugNetworkingArbitrerMessage_ClientServerData::Client_GetCanConnect() const
{
    return mCanConnect;
}