// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Header/KLDebugNetworkingMessage_Header.h"

FKLDebugNetworkingMessage_Header::FKLDebugNetworkingMessage_Header(const uint16 _MessageType, const uint16 _MessageEnumType, const uint32 _MessageDataSize, const uint32 _TotalUncompressDataSize, const bool _IsCompressed, const uint32 _TotalCompressDataSize, const uint32 _StartCompressDataOffset, const uint32 _MessageDataOffset, const KL::Debug::Networking::Message::MessageID _MessageID)
    : mMessageMark(KL::Debug::Networking::Message::MessageMark)
    , mMessageDataSize(_MessageDataSize)
    , mTotalUncompressDataSize(_TotalUncompressDataSize)
    , mTotalCompressedDataSize(_TotalCompressDataSize)
    , mStartCompressDataOffset(_StartCompressDataOffset)
    , mMessageDataOffset(_MessageDataOffset)
    , mMessageType(_MessageType)
    , mMessageEnumType(_MessageEnumType)
    , mMessageID(_MessageID)
    , mIsCompressed(_IsCompressed)
{
}

FKLDebugNetworkingMessage_Header::FKLDebugNetworkingMessage_Header(FArchive& _Archive)
{
    Serialize(_Archive);
}

bool FKLDebugNetworkingMessage_Header::IsValid() const
{
    return mMessageMark == KL::Debug::Networking::Message::MessageMark;
}

void FKLDebugNetworkingMessage_Header::Serialize(FArchive& _Archive)
{
    _Archive << mMessageMark;
    _Archive << mMessageDataSize;
    _Archive << mTotalUncompressDataSize;
    _Archive << mTotalCompressedDataSize;
    _Archive << mStartCompressDataOffset;
    _Archive << mMessageDataOffset;
    _Archive << mMessageType;
    _Archive << mMessageEnumType;
    _Archive << mIsCompressed;
    _Archive << mMessageID;
}
