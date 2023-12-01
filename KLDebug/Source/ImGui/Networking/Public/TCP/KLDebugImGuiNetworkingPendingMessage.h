// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Message/Header/KLDebugImGuiNetworkingMessage_Header.h"
#include "ImGui/Networking/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingPendingMessage final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworkingPendingMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData);
    explicit FKLDebugImGuiNetworkingPendingMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, TArray<uint8>&& _MessageData);

    UE_NODISCARD bool HasData() const;
    UE_NODISCARD EKLDebugNetworkMessageTypes GetMesageType() const;
    UE_NODISCARD const TArray<uint8>& GetMessageData() const;

private:
    void UncompressData(const FKLDebugImGuiNetworkingMessage_Header& _Header, const TArray<uint8>& _MessageData);

private:
    EKLDebugNetworkMessageTypes mMessageType;
    TArray<uint8> mData;
};

inline EKLDebugNetworkMessageTypes FKLDebugImGuiNetworkingPendingMessage::GetMesageType() const
{
    return mMessageType;
}

inline const TArray<uint8>& FKLDebugImGuiNetworkingPendingMessage::GetMessageData() const
{
    return mData;
}

inline bool FKLDebugImGuiNetworkingPendingMessage::HasData() const
{
    return !mData.IsEmpty();
}