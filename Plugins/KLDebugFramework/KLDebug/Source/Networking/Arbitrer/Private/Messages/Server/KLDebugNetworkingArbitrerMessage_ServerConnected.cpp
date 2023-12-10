#include "Messages/Server/KLDebugNetworkingArbitrerMessage_ServerConnected.h"

// modules
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessageHeaderDefinitions.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingArbitrerMessage_ServerConnected::FKLDebugNetworkingArbitrerMessage_ServerConnected(const uint32 _WorldServerPort, const uint32 _DebugPort)
    : mWorldServerPort(_WorldServerPort)
    , mDebugPort(_DebugPort)
    , mHeaderVersion(static_cast<uint8>(EMessageHeaderVersion::Latest))
{
}

FKLDebugNetworkingArbitrerMessage_ServerConnected::FKLDebugNetworkingArbitrerMessage_ServerConnected(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingArbitrerMessage_ServerConnected::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mWorldServerPort;
    Archive << mDebugPort;
    Archive << mHeaderVersion;
}