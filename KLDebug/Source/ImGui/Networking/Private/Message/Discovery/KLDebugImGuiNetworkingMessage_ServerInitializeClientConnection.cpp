// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Discovery/KLDebugImGuiNetworkingMessage_ServerInitializeClientConnection.h"

#include "Message/Header/KLDebugImGuiNetworkingMessageHeaderDefinitions.h"
#include "Message/KLDebugImGuiNetworkingMessageTypes.h"
#include "Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection::FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection(const FNetworkGUID& _ServerWorldNetworkGUID, const FNetworkGUID& _PlayerContollerNetworkGUID)
    : mServerWorldGUID(_ServerWorldNetworkGUID)
    , mPlayerContollerGUID(_PlayerContollerNetworkGUID)
    , mServerHeaderVersion(static_cast<uint8>(EMessageHeaderVersion::Latest))
{
}

FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection::FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mServerHeaderVersion;
    Archive << mServerWorldGUID;
    Archive << mPlayerContollerGUID;
}

bool FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection::IsValid() const
{
    return mServerHeaderVersion == static_cast<uint8>(EMessageHeaderVersion::Latest);
}
