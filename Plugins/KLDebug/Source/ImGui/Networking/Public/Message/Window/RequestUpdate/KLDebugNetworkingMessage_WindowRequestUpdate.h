// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Window/KLDebugNetworkingMessage_WindowBase.h"

// engine
#include "Containers/Array.h"
#include "HAL/Platform.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGIMGUINETWORKING_API FKLDebugNetworkingMessage_WindowRequestUpdate final : public TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Client_WindowRequestUpdate>
{
public:
    explicit FKLDebugNetworkingMessage_WindowRequestUpdate() = default;
    explicit FKLDebugNetworkingMessage_WindowRequestUpdate(const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType, const TArray<uint8>& _Buffer);

    UE_NODISCARD TArray<uint8>& Server_GetBufferDataMutable();

private:
    // TKLDebugNetworkingMessage_Base
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // TKLDebugNetworkingMessage_Base

private:
    TArray<uint8> mBufferData;
};

inline TArray<uint8>& FKLDebugNetworkingMessage_WindowRequestUpdate::Server_GetBufferDataMutable()
{
    return mBufferData;
}