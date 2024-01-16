// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Window/KLDebugNetworkingMessage_WindowBase.h"

// modules
#include "User/Networking/Public/Message/KLDebugNetworkingMessageTypes.h"

// engine
#include "Containers/Array.h"
#include "HAL/Platform.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGIMGUINETWORKING_API FKLDebugNetworkingMessage_WindowDataUpdate final : public TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Server_WindowDataUpdate>
{
public:
    explicit FKLDebugNetworkingMessage_WindowDataUpdate(const EKLDebugNetworkReceiveMessageType _ReceiveType, const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType, const TArray<uint8>& _Buffer);
    explicit FKLDebugNetworkingMessage_WindowDataUpdate(FArchive& _Archive);

    UE_NODISCARD TArray<uint8>& Client_GetBufferDataMutable();
    UE_NODISCARD EKLDebugNetworkReceiveMessageType GetReceiveType() const;

private:
    // TKLDebugNetworkingMessage_Base
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // TKLDebugNetworkingMessage_Base

private:
    TArray<uint8> mBufferData;
    EKLDebugNetworkReceiveMessageType mReceiveType = EKLDebugNetworkReceiveMessageType::RequestUpdate;
};

inline TArray<uint8>& FKLDebugNetworkingMessage_WindowDataUpdate::Client_GetBufferDataMutable()
{
    return mBufferData;
}

inline EKLDebugNetworkReceiveMessageType FKLDebugNetworkingMessage_WindowDataUpdate::GetReceiveType() const
{
    return mReceiveType;
}