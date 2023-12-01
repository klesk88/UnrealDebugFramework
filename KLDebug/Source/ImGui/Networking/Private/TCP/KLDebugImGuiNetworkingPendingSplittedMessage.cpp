// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "TCP/KLDebugImGuiNetworkingPendingSplittedMessage.h"

FKLDebugImGuiNetworkingPendingSplittedMessage::FKLDebugImGuiNetworkingPendingSplittedMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, const TArrayView<uint8>& _MessageData)
    : mHeader(_Header)
{
    mData.AddUninitialized(_Header.GetTotalMessageDataExpected());
    static_cast<void>(AddData(_MessageData, _Header.GetMessageDataOffset()));
}

void FKLDebugImGuiNetworkingPendingSplittedMessage::AddData(const TArrayView<uint8>& _MessageData, const uint32 _OffsetIndex)
{
    const int32 DataCountToWrite = _OffsetIndex + _MessageData.Num();
    if (DataCountToWrite > mData.Num() || mCouldNotReadData)
    {
        ensureMsgf(!mCouldNotReadData, TEXT("out of bounds cant store data"));
        mCouldNotReadData = true;
        return;
    }

    FMemory::Memcpy(&mData[_OffsetIndex], _MessageData.GetData(), _MessageData.Num());
    TotalSizeReceived += _MessageData.Num();
}
