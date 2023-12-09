// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Math/NumericLimits.h"

class FArchive;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingMessage_Header
{
public:
    explicit FKLDebugNetworkingMessage_Header(const uint16 _MessageType, const uint16 _MessageEnumType, const uint32 _MessageDataSize, const uint32 _TotalUncompressDataSize, const bool _IsCompressed, const uint32 _TotalCompressDataSize, const uint32 _StartCompressDataOffset, const uint32 _MessageDataOffset, const KL::Debug::Networking::Message::MessageID _MessageID);
    explicit FKLDebugNetworkingMessage_Header(FArchive& _Archive);

    void Serialize(FArchive& _Archive);

    UE_NODISCARD bool IsValid() const;

    UE_NODISCARD bool IsDataCompressed() const;
    UE_NODISCARD bool HasReceivedAllData() const;

    UE_NODISCARD bool IsSplittedMessage() const;
    // it will check if we received all the data from the network in case of splitted messages.
    // It will also take already into account if the data has been compressed or no
    UE_NODISCARD bool IsFullyReceived(const uint32 _TotalDataReceived) const;
    UE_NODISCARD uint32 GetTotalMessageDataExpected() const;

    UE_NODISCARD uint32 GetMessageDataSize() const;
    UE_NODISCARD uint32 GetTotalUncompressDataSize() const;
    UE_NODISCARD uint32 GetTotalCompressDataSize() const;
    UE_NODISCARD uint32 GetStartCompressDataOffset() const;
    UE_NODISCARD uint32 GetMessageDataOffset() const;
    UE_NODISCARD uint16 GetMessageType() const;
    UE_NODISCARD uint16 GetMessageEnumType() const;
    UE_NODISCARD uint16 GetMessageID() const;

private:
    uint32 mMessageMark = TNumericLimits<uint32>::Max();
    uint32 mMessageDataSize = 0;
    uint32 mTotalUncompressDataSize = 0;
    uint32 mTotalCompressedDataSize = 0;
    uint32 mStartCompressDataOffset = 0;
    uint32 mMessageDataOffset = 0;
    uint16 mMessageType = 0;
    uint16 mMessageEnumType = TNumericLimits<uint16>::Max();
    KL::Debug::Networking::Message::MessageID mMessageID = 0;
    bool mIsCompressed = false;
};

inline uint32 FKLDebugNetworkingMessage_Header::GetMessageDataSize() const
{
    return mMessageDataSize;
}

inline uint32 FKLDebugNetworkingMessage_Header::GetTotalUncompressDataSize() const
{
    return mTotalUncompressDataSize;
}

inline uint32 FKLDebugNetworkingMessage_Header::GetTotalCompressDataSize() const
{
    return mTotalCompressedDataSize;
}

inline uint32 FKLDebugNetworkingMessage_Header::GetStartCompressDataOffset() const
{
    return mStartCompressDataOffset;
}

inline uint32 FKLDebugNetworkingMessage_Header::GetMessageDataOffset() const
{
    return mMessageDataOffset;
}

inline uint16 FKLDebugNetworkingMessage_Header::GetMessageType() const
{
    return mMessageType;
}

inline uint16 FKLDebugNetworkingMessage_Header::GetMessageEnumType() const
{
    return mMessageEnumType;
}

inline uint16 FKLDebugNetworkingMessage_Header::GetMessageID() const
{
    return mMessageID;
}

inline bool FKLDebugNetworkingMessage_Header::IsDataCompressed() const
{
    return mIsCompressed;
}

inline bool FKLDebugNetworkingMessage_Header::HasReceivedAllData() const
{
    return mMessageDataSize == mTotalUncompressDataSize;
}

inline uint32 FKLDebugNetworkingMessage_Header::GetTotalMessageDataExpected() const
{
    return IsDataCompressed() ? mTotalCompressedDataSize : mTotalUncompressDataSize;
}

inline bool FKLDebugNetworkingMessage_Header::IsSplittedMessage() const
{
    const uint32 TotalData = GetTotalMessageDataExpected();
    ensureMsgf(mMessageDataSize <= TotalData, TEXT("we read more data then expected"));
    return mMessageDataSize != TotalData;
}

inline bool FKLDebugNetworkingMessage_Header::IsFullyReceived(const uint32 _TotalDataReceived) const
{
    const uint32 TotalData = GetTotalMessageDataExpected();
    ensureMsgf(_TotalDataReceived <= TotalData, TEXT("we read more data then expected"));
    return _TotalDataReceived == TotalData;
}
