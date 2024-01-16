// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Window/DataUpdate/KLDebugNetworkingMessage_WindowDataUpdate.h"

FKLDebugNetworkingMessage_WindowDataUpdate::FKLDebugNetworkingMessage_WindowDataUpdate(const EKLDebugNetworkReceiveMessageType _ReceiveType, const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType, const TArray<uint8>& _Buffer)
    : TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Server_WindowDataUpdate>(_NewSelection, _ID, _WindowType)
    , mBufferData(_Buffer)
    , mReceiveType(_ReceiveType)
{
}

FKLDebugNetworkingMessage_WindowDataUpdate::FKLDebugNetworkingMessage_WindowDataUpdate(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingMessage_WindowDataUpdate::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Server_WindowDataUpdate>::SerializeChild(_Input);

    _Input.GetArchive() << mBufferData;
    _Input.GetArchive() << mReceiveType;

    ensureMsgf(!mBufferData.IsEmpty(), TEXT("there should be data"));
}
