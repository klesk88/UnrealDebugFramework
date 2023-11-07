#pragma once

#include "Message/KLDebugImGuiNetworkingMessage_Base.h"

//engine
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Math/NumericLimits.h"
#include "Misc/NetworkGuid.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection final : public TKLDebugImGuiNetworkingMessage_Base<EKLDebugNetworkMessageTypes::Server_InitClientConnection>
{
public:
    explicit FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection(const FNetworkGUID& _ServerWorldNetworkGUID, const FNetworkGUID& _PlayerContollerNetworkGUID);
    explicit FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection(FArchive& _Archive);

    UE_NODISCARD bool IsValid() const;

    UE_NODISCARD const FNetworkGUID& Client_GetServerWorld() const;
    UE_NODISCARD const FNetworkGUID& Client_GetPlayerController() const;
    UE_NODISCARD uint8 Client_GetServerHeaderVersion() const;

private:
    //IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    //IKLDebugNetworkingMessageInterface

private:
    FNetworkGUID mServerWorldGUID;
    FNetworkGUID mPlayerContollerGUID;
    uint8 mServerHeaderVersion = TNumericLimits<uint32>::Max();
};

inline uint8 FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection::Client_GetServerHeaderVersion() const
{
    return mServerHeaderVersion;
}

inline const FNetworkGUID& FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection::Client_GetServerWorld() const
{
    return mServerWorldGUID;
}

inline const FNetworkGUID& FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection::Client_GetPlayerController() const
{
    return mPlayerContollerGUID;
}

