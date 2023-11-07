#pragma once

#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

//engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Math/NumericLimits.h"

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_Header
{
public:
    explicit FKLDebugImGuiNetworkingMessage_Header(const uint16 _MessageType, const uint8 _MessageVersion, const uint32 _MessageDataSize, const uint32 _TotalUncompressDataSize, const bool _IsCompressed, const uint32 _TotalCompressDataSize, const uint32 _StartCompressDataOffset, const uint32 _MessageDataOffset, const KL::Debug::ImGui::Networking::Message::MessageID _MessageID);
    explicit FKLDebugImGuiNetworkingMessage_Header(FArchive& _Archive);

    void Serialize(FArchive& _Archive);

    UE_NODISCARD bool IsValid() const;

    UE_NODISCARD bool IsDataCompressed() const;
    UE_NODISCARD bool HasReceivedAllData() const;

    UE_NODISCARD bool IsSplittedMessage() const;
    //it will check if we received all the data from the network in case of splitted messages.
    //It will also take already into account if the data has been compressed or no
    UE_NODISCARD bool IsFullyReceived(const uint32 _TotalDataReceived) const;
    UE_NODISCARD uint32 GetTotalMessageDataExpected() const;

    UE_NODISCARD uint32 GetMessageDataSize() const;
    UE_NODISCARD uint32 GetTotalUncompressDataSize() const;
    UE_NODISCARD uint32 GetTotalCompressDataSize() const;
    UE_NODISCARD uint32 GetStartCompressDataOffset() const;
    UE_NODISCARD uint32 GetMessageDataOffset() const;
    UE_NODISCARD uint16 GetMessageType() const;
    UE_NODISCARD uint16 GetMessageID() const;

private:
    uint32 mMessageMark = TNumericLimits<uint32>::Max();
    uint32 mMessageDataSize = 0;
    uint32 mTotalUncompressDataSize = 0;
    uint32 mTotalCompressedDataSize = 0;
    uint32 mStartCompressDataOffset = 0;
    uint32 mMessageDataOffset = 0;
    uint16 mMessageType = 0;
    bool mIsCompressed = false;
    KL::Debug::ImGui::Networking::Message::MessageID mMessageID = 0;
};

inline uint32 FKLDebugImGuiNetworkingMessage_Header::GetMessageDataSize() const
{
    return mMessageDataSize;
}

inline uint32 FKLDebugImGuiNetworkingMessage_Header::GetTotalUncompressDataSize() const
{
    return mTotalUncompressDataSize;
}

inline uint32 FKLDebugImGuiNetworkingMessage_Header::GetTotalCompressDataSize() const
{
    return mTotalCompressedDataSize;
}

inline uint32 FKLDebugImGuiNetworkingMessage_Header::GetStartCompressDataOffset() const
{
    return mStartCompressDataOffset;
}

inline uint32 FKLDebugImGuiNetworkingMessage_Header::GetMessageDataOffset() const
{
    return mMessageDataOffset;
}

inline uint16 FKLDebugImGuiNetworkingMessage_Header::GetMessageType() const
{
    return mMessageType;
}

inline uint16 FKLDebugImGuiNetworkingMessage_Header::GetMessageID() const
{
    return mMessageID;
}

inline bool FKLDebugImGuiNetworkingMessage_Header::IsDataCompressed() const
{
    return mIsCompressed;
}

inline bool FKLDebugImGuiNetworkingMessage_Header::HasReceivedAllData() const
{
    return mMessageDataSize == mTotalUncompressDataSize;
}

inline uint32 FKLDebugImGuiNetworkingMessage_Header::GetTotalMessageDataExpected() const
{
    return IsDataCompressed() ? mTotalCompressedDataSize : mTotalUncompressDataSize;
}

inline bool FKLDebugImGuiNetworkingMessage_Header::IsSplittedMessage() const
{
    const uint32 TotalData = GetTotalMessageDataExpected();
    ensureMsgf(mMessageDataSize <= TotalData, TEXT("we read more data then expected"));
    return mMessageDataSize != TotalData;
}

inline bool FKLDebugImGuiNetworkingMessage_Header::IsFullyReceived(const uint32 _TotalDataReceived) const
{
    const uint32 TotalData = GetTotalMessageDataExpected();
    ensureMsgf(_TotalDataReceived <= TotalData, TEXT("we read more data then expected"));
    return _TotalDataReceived == TotalData;
}