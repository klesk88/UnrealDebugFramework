// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Messages/Server/KLDebugNetworkingArbitrerMessage_ServerPong.h"

// modules
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessageHeaderDefinitions.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingArbitrerMessage_ServerPong::FKLDebugNetworkingArbitrerMessage_ServerPong(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingArbitrerMessage_ServerPong::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
}
