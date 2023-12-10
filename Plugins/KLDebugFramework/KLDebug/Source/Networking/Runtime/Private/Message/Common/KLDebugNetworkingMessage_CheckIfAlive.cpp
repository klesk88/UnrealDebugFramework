// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Common/KLDebugNetworkingMessage_CheckIfAlive.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

FKLDebugNetworkingMessage_CheckIfAlive::FKLDebugNetworkingMessage_CheckIfAlive(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingMessage_CheckIfAlive::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
}
