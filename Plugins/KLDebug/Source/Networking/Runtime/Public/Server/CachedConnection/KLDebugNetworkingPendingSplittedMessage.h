// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "HAL/UnrealMemory.h"
#include "Templates/UnrealTemplate.h"

// these are messages that have been splitted server side and we are waiting to receive all packets for them
class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingPendingSplittedMessage final : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingPendingSplittedMessage(const FKLDebugNetworkingMessage_Header& _Header, const TArrayView<uint8>& _MessageData);
    [[nodiscard]] bool operator==(const KL::Debug::Networking::Message::MessageID _MessageID) const;

    void AddData(const TArrayView<uint8>& _MessageData, const uint32 _OffsetIndex);
    [[nodiscard]] bool IsFullyReceived() const;
    [[nodiscard]] bool HasFailedToReadData() const;

    [[nodiscard]] const FKLDebugNetworkingMessage_Header& GetHeaderMessage() const;
    [[nodiscard]] TArray<uint8>& GetDataMutable();

private:
    FKLDebugNetworkingMessage_Header mHeader;
    TArray<uint8> mData;
    uint32 TotalSizeReceived = 0;
    bool mCouldNotReadData = false;
};

inline bool FKLDebugNetworkingPendingSplittedMessage::operator==(const KL::Debug::Networking::Message::MessageID _MessageID) const
{
    return mHeader.GetMessageID() == _MessageID;
}

inline bool FKLDebugNetworkingPendingSplittedMessage::IsFullyReceived() const
{
    return mHeader.IsFullyReceived(TotalSizeReceived);
}

inline bool FKLDebugNetworkingPendingSplittedMessage::HasFailedToReadData() const
{
    return mCouldNotReadData;
}

inline const FKLDebugNetworkingMessage_Header& FKLDebugNetworkingPendingSplittedMessage::GetHeaderMessage() const
{
    return mHeader;
}

inline TArray<uint8>& FKLDebugNetworkingPendingSplittedMessage::GetDataMutable()
{
    return mData;
}