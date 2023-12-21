// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Messages/Server/KLDebugNetworkingArbitrerMessage_ServerPing.h"

// modules
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessageHeaderDefinitions.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingArbitrerMessage_ServerPing::FKLDebugNetworkingArbitrerMessage_ServerPing(const uint32 _AnswerPort)
    : mAnswerPort(_AnswerPort)
{
}

FKLDebugNetworkingArbitrerMessage_ServerPing::FKLDebugNetworkingArbitrerMessage_ServerPing(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingArbitrerMessage_ServerPing::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mAnswerPort;
}
