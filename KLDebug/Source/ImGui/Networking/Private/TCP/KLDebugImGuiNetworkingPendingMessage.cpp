#include "TCP/KLDebugImGuiNetworkingPendingMessage.h"

#include "Message/Header/KLDebugImGuiNetworkingMessage_Header.h"
#include "Message/Helpers/KLDebugImGuiNetworkingMessage_Helpers.h"

//engine
#include "HAL/UnrealMemory.h"

FKLDebugImGuiNetworkingPendingMessage::FKLDebugImGuiNetworkingPendingMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData)
    : mMessageType(static_cast<EKLDebugNetworkMessageTypes>(_Header.GetMessageType()))
{
    if (_Header.IsDataCompressed())
    {
        UncompressData(_Header, _MessageData);
    }
    else
    {
        mData = _MessageData;
    }
}

FKLDebugImGuiNetworkingPendingMessage::FKLDebugImGuiNetworkingPendingMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, TArray<uint8>&& _MessageData)
    : mMessageType(static_cast<EKLDebugNetworkMessageTypes>(_Header.GetMessageType()))
{
    if (_Header.IsDataCompressed())
    {
        UncompressData(_Header, _MessageData);
    }
    else
    {
        mData = MoveTemp(_MessageData);
    }
}

void FKLDebugImGuiNetworkingPendingMessage::UncompressData(const FKLDebugImGuiNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData)
{
    if (!_MessageData.IsValidIndex(_Header.GetStartCompressDataOffset()))
    {
        ensureMsgf(false, TEXT("compress start offset should be valid"));
        return;
    }

    mData.AddUninitialized(static_cast<int32>(_Header.GetTotalUncompressDataSize()));
    FMemory::Memcpy(&mData[0], _MessageData.GetData(), _Header.GetStartCompressDataOffset());
    const int32 CompressSize = _MessageData.Num() - _Header.GetStartCompressDataOffset();
    const TArrayView<const uint8> CompressData(&_MessageData[_Header.GetStartCompressDataOffset()], CompressSize);

    const int32 UncompressDataSize = mData.Num() - _Header.GetStartCompressDataOffset();
    TArrayView<uint8> UncompressDataToFill(&mData[_Header.GetStartCompressDataOffset()], UncompressDataSize);
    const bool Result = KL::Debug::ImGuiNetworking::Message::UncompressBuffer(UncompressDataSize, CompressData, UncompressDataToFill);
    if (!Result)
    {
        ensureMsgf(false, TEXT("failed to uncomrpess data"));
        mData.Empty();
    }
}
