// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Header/KLDebugNetworkingMessage_Header.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingPendingMessage final : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingPendingMessage(const FKLDebugNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData);
    explicit FKLDebugNetworkingPendingMessage(const FKLDebugNetworkingMessage_Header& _Header, TArray<uint8>&& _MessageData);
    explicit FKLDebugNetworkingPendingMessage(FKLDebugNetworkingPendingMessage&& _Other);

    UE_NODISCARD bool HasData() const;
    UE_NODISCARD uint16 GetMessageType() const;
    UE_NODISCARD uint16 GetMessageEnumType() const;
    UE_NODISCARD const TArray<uint8>& GetMessageData() const;

private:
    void UncompressData(const FKLDebugNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData);

private:
    uint16 mMessageType;
    uint16 mMessageEnumType;
    TArray<uint8> mData;
};

inline uint16 FKLDebugNetworkingPendingMessage::GetMessageType() const
{
    return mMessageType;
}

inline uint16 FKLDebugNetworkingPendingMessage::GetMessageEnumType() const
{
    return mMessageEnumType;
}

inline const TArray<uint8>& FKLDebugNetworkingPendingMessage::GetMessageData() const
{
    return mData;
}

inline bool FKLDebugNetworkingPendingMessage::HasData() const
{
    return !mData.IsEmpty();
}