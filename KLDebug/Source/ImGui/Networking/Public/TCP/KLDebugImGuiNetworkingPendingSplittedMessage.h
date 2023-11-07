#pragma once

#include "Message/Header/KLDebugImGuiNetworkingMessage_Header.h"
#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

//engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "HAL/UnrealMemory.h"
#include "Templates/UnrealTemplate.h"

//these are messages that have been splitted server side and we are waiting to receive all packets for them
class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingPendingSplittedMessage final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworkingPendingSplittedMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, const TArrayView<uint8>& _MessageData);
    UE_NODISCARD bool operator==(const KL::Debug::ImGui::Networking::Message::MessageID _MessageID) const;

    void AddData(const TArrayView<uint8>& _MessageData, const uint32 _OffsetIndex);
    UE_NODISCARD bool IsFullyReceived() const;
    UE_NODISCARD bool HasFailedToReadData() const;

    UE_NODISCARD const FKLDebugImGuiNetworkingMessage_Header& GetHeaderMessage() const;
    UE_NODISCARD TArray<uint8>& GetDataMutable();

private:
    FKLDebugImGuiNetworkingMessage_Header mHeader;
    TArray<uint8> mData;
    uint32 TotalSizeReceived = 0;
    bool mCouldNotReadData = false;
};

inline bool FKLDebugImGuiNetworkingPendingSplittedMessage::operator==(const KL::Debug::ImGui::Networking::Message::MessageID _MessageID) const
{
    return mHeader.GetMessageID() == _MessageID;
}

inline bool FKLDebugImGuiNetworkingPendingSplittedMessage::IsFullyReceived() const
{
    return mHeader.IsFullyReceived(TotalSizeReceived);
}

inline bool FKLDebugImGuiNetworkingPendingSplittedMessage::HasFailedToReadData() const
{
    return mCouldNotReadData;
}

inline const FKLDebugImGuiNetworkingMessage_Header& FKLDebugImGuiNetworkingPendingSplittedMessage::GetHeaderMessage() const
{
    return mHeader;
}

inline TArray<uint8>& FKLDebugImGuiNetworkingPendingSplittedMessage::GetDataMutable()
{
    return mData;
}