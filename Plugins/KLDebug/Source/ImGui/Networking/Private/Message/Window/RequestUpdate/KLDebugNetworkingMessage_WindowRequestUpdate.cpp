// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Window/RequestUpdate/KLDebugNetworkingMessage_WindowRequestUpdate.h"

FKLDebugNetworkingMessage_WindowRequestUpdate::FKLDebugNetworkingMessage_WindowRequestUpdate(const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType, const TArray<uint8>& _Buffer)
    : TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Client_WindowRequestUpdate>(_NewSelection, _ID, _WindowType)
    , mBufferData(_Buffer)
{
}

FKLDebugNetworkingMessage_WindowRequestUpdate::FKLDebugNetworkingMessage_WindowRequestUpdate(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugNetworkingMessage_WindowRequestUpdate::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Client_WindowRequestUpdate>::SerializeChild(_Input);

    _Input.GetArchive() << mBufferData;

    ensureMsgf(!mBufferData.IsEmpty(), TEXT("there should be data"));
}
