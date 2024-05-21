// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugNetworkingMessage_Base.h"
#include "Message/KLDebugNetworkingMessageDefinitions.h"

// engine
#include "GameFramework/OnlineReplStructs.h"
#include "Misc/NetworkGuid.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGIMGUINETWORKING_API FKLDebugNetworkingMessage_ClientConnected final : public TKLDebugNetworkingMessage_Base<EKLDebugNetworkingMessage, EKLDebugNetworkingMessage::ClientConnected, static_cast<uint16>(EKLDebugNetworkingMessagesTypes::NetworkingRuntime)>
{
public:
    explicit FKLDebugNetworkingMessage_ClientConnected(const FUniqueNetIdRepl& _LocalPlayerNetID);
    explicit FKLDebugNetworkingMessage_ClientConnected(FArchive& _Archive);

    [[nodiscard]] const FUniqueNetIdRepl& GetPlayerNetID() const;

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface

private:
    FUniqueNetIdRepl mLocalPlayerNetID;
};

inline const FUniqueNetIdRepl& FKLDebugNetworkingMessage_ClientConnected::GetPlayerNetID() const
{
    return mLocalPlayerNetID;
}