// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Messages/Server/KLDebugNetworkingArbitrerMessage_ServerDisconnected.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingArbitrerMessage_ServerDisconnected::FKLDebugNetworkingArbitrerMessage_ServerDisconnected(const uint32 _WorldServerPort)
    : mWorldServerPort(_WorldServerPort)
{
}

FKLDebugNetworkingArbitrerMessage_ServerDisconnected::FKLDebugNetworkingArbitrerMessage_ServerDisconnected(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingArbitrerMessage_ServerDisconnected::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mWorldServerPort;
}