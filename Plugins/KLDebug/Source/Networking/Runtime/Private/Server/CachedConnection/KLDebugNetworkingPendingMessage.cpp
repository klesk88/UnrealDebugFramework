// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/CachedConnection/KLDebugNetworkingPendingMessage.h"

#include "Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Containers/ArrayView.h"
#include "HAL/UnrealMemory.h"

FKLDebugNetworkingPendingMessage::FKLDebugNetworkingPendingMessage(const FKLDebugNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData)
    : mMessageType(_Header.GetMessageType())
    , mMessageEnumType(_Header.GetMessageEnumType())
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

FKLDebugNetworkingPendingMessage::FKLDebugNetworkingPendingMessage(const FKLDebugNetworkingMessage_Header& _Header, TArray<uint8>&& _MessageData)
    : mMessageType(_Header.GetMessageType())
    , mMessageEnumType(_Header.GetMessageEnumType())
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

FKLDebugNetworkingPendingMessage::FKLDebugNetworkingPendingMessage(FKLDebugNetworkingPendingMessage&& _Other)
    : mMessageType(_Other.GetMessageType())
    , mMessageEnumType(_Other.GetMessageEnumType())
    , mData(MoveTemp(_Other.mData))
{
}

void FKLDebugNetworkingPendingMessage::UncompressData(const FKLDebugNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData)
{
    if (!_MessageData.IsValidIndex(_Header.GetStartCompressDataOffset()))
    {
        ensureMsgf(false, TEXT("compress start offset should be valid"));
        return;
    }

    mData.AddUninitialized(static_cast<int32>(_Header.GetTotalUncompressDataSize()));
    if (_Header.GetStartCompressDataOffset() != 0)
    {
        FMemory::Memcpy(&mData[0], _MessageData.GetData(), _Header.GetStartCompressDataOffset());
    }

    const int32 CompressSize = _MessageData.Num() - _Header.GetStartCompressDataOffset();
    const TArrayView<const uint8> CompressData(&_MessageData[_Header.GetStartCompressDataOffset()], CompressSize);

    const int32 UncompressDataSize = mData.Num() - _Header.GetStartCompressDataOffset();
    TArrayView<uint8> UncompressDataToFill(&mData[_Header.GetStartCompressDataOffset()], UncompressDataSize);
    const bool Result = KL::Debug::Networking::Message::UncompressBuffer(UncompressDataSize, CompressData, UncompressDataToFill);
    if (!Result)
    {
        ensureMsgf(false, TEXT("failed to uncomrpess data"));
        mData.Empty();
    }
}
