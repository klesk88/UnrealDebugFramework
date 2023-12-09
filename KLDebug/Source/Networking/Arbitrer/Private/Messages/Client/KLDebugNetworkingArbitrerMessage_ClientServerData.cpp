#include "Messages/Client/KLDebugNetworkingArbitrerMessage_ClientServerData.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugNetworkingArbitrerMessage_ClientServerData::FKLDebugNetworkingArbitrerMessage_ClientServerData(const uint32 _DebugPort, const uint16 _ClientID, const bool _CanConnect)
    : mDebugPort(_DebugPort)
    , mClientID(_ClientID)
    , mCanConnect(_CanConnect)
{
}

FKLDebugNetworkingArbitrerMessage_ClientServerData::FKLDebugNetworkingArbitrerMessage_ClientServerData(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingArbitrerMessage_ClientServerData::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mDebugPort;
    Archive << mClientID;
    Archive << mCanConnect;
}
