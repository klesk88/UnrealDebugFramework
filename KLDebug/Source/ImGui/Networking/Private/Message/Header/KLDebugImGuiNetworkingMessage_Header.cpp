#include "Message/Header/KLDebugImGuiNetworkingMessage_Header.h"

#include "Message/Header/KLDebugImGuiNetworkingMessageHeaderDefinitions.h"

FKLDebugImGuiNetworkingMessage_Header::FKLDebugImGuiNetworkingMessage_Header(const uint16 _MessageType, const uint8 _MessageVersion, const uint32 _MessageDataSize, const uint32 _TotalUncompressDataSize, const bool _IsCompressed, const uint32 _TotalCompressDataSize, const uint32 _StartCompressDataOffset, const uint32 _MessageDataOffset, const KL::Debug::ImGui::Networking::Message::MessageID _MessageID)
    : mMessageMark(KL::Debug::ImGui::Networking::Message::MessageMark)
    , mMessageDataSize(_MessageDataSize)
    , mTotalUncompressDataSize(_TotalUncompressDataSize)
    , mTotalCompressedDataSize(_TotalCompressDataSize)
    , mStartCompressDataOffset(_StartCompressDataOffset)
    , mMessageDataOffset(_MessageDataOffset)
    , mMessageType(_MessageType)
    , mIsCompressed(_IsCompressed)
    , mMessageID(_MessageID)
{
}

FKLDebugImGuiNetworkingMessage_Header::FKLDebugImGuiNetworkingMessage_Header(FArchive& _Archive)
{
    Serialize(_Archive);
}

bool FKLDebugImGuiNetworkingMessage_Header::IsValid() const
{
    return mMessageMark == KL::Debug::ImGui::Networking::Message::MessageMark;
}

void FKLDebugImGuiNetworkingMessage_Header::Serialize(FArchive& _Archive)
{
    _Archive << mMessageMark;
    _Archive << mMessageDataSize;
    _Archive << mTotalUncompressDataSize;
    _Archive << mTotalCompressedDataSize;
    _Archive << mStartCompressDataOffset;
    _Archive << mMessageDataOffset;
    _Archive << mMessageType;
    _Archive << mIsCompressed;
    _Archive << mMessageID;
}
