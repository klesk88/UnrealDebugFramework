// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Message/KLDebugNetworkingMessage_Command.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingMessage_Command::FKLDebugNetworkingMessage_Command(const FName& _ID)
    : mCommandID(_ID)
{
}

FKLDebugNetworkingMessage_Command::FKLDebugNetworkingMessage_Command(const FName& _ID, TArray<uint8>&& _ContextData)
    : mCommandID(_ID)
    , mCommandContextData(MoveTemp(_ContextData))
{
}

FKLDebugNetworkingMessage_Command::FKLDebugNetworkingMessage_Command(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingMessage_Command::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mCommandID;
    Archive << mCommandContextData;
}
