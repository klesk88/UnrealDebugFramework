// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Client/KLDebugNetworkingMessage_ClientConnected.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingMessage_ClientConnected::FKLDebugNetworkingMessage_ClientConnected(const FUniqueNetIdRepl& _LocalPlayerNetID)
    : mLocalPlayerNetID(_LocalPlayerNetID)
{
}

FKLDebugNetworkingMessage_ClientConnected::FKLDebugNetworkingMessage_ClientConnected(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingMessage_ClientConnected::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mLocalPlayerNetID;
}
