#include "Messages/Client/KLDebugNetworkingArbitrerMessage_ClientConnected.h"

// modules
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessageHeaderDefinitions.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingArbitrerMessage_ClientConnected::FKLDebugNetworkingArbitrerMessage_ClientConnected(const FString& _Host, const uint32 _WorldServerPort, const uint32 _ArbitrerReplyPort, const uint16 _ID)
    : mHost(_Host)
    , mWorldServerPort(_WorldServerPort)
    , mArbitrerReplyPort(_ArbitrerReplyPort)
    , mClientID(_ID)
    , mHeaderVersion(static_cast<uint8>(EMessageHeaderVersion::Latest))
{
}

FKLDebugNetworkingArbitrerMessage_ClientConnected::FKLDebugNetworkingArbitrerMessage_ClientConnected(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingArbitrerMessage_ClientConnected::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mHost;
    Archive << mWorldServerPort;
    Archive << mArbitrerReplyPort;
    Archive << mClientID;
    Archive << mHeaderVersion;
}
