// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientManager.h"
#include "Commands/Manager/KLDebugNetworkingClientCommandConnectionManager.h"

// modules
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingCachedConnectionBase.h"

// engine
#include "Containers/Array.h"
#include "GameFramework/OnlineReplStructs.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"

class FArchive;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientServerCacheConnection final : public FKLDebugNetworkingCachedConnectionBase
{
private:
    enum class EClientState : uint8
    {
        SendClientInit = 0,
        FullyInitialized,

        Failure
    };

public:
    explicit FKLDebugImGuiClientServerCacheConnection(const FObjectKey& _WorldKey, const FUniqueNetIdRepl& _LocalPlayerNetID, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket);

    // FKLDebugImGuiNetworkingTCPCachedConnectionBase
    UE_NODISCARD bool IsValid() const final;
    UE_NODISCARD bool RequiresGameThreadTick() const final;
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase

    UE_NODISCARD bool TickOnGameThread(UWorld& _World);

private:
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase
    UE_NODISCARD bool TickChild() final;
    void TickChildWriteBuffer(FArchive& _Writer) final;
    void Parallel_HandlePendingMessageChild(FKLDebugNetworkingPendingMessage&& _PendingMessage) final;
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase

private:
    FKLDebugNetworkingClientCommandConnectionManager mCommandsManager;
    FKLDebugImGuiClientManager mClientImGuiData;
    FUniqueNetIdRepl mLocalPlayerNetID;
    EClientState mState = EClientState::SendClientInit;
    bool mIsWorldValid = true;
};

inline bool FKLDebugImGuiClientServerCacheConnection::RequiresGameThreadTick() const
{
    return mClientImGuiData.RequiresGameThreadTick() || mCommandsManager.HasPendingData();
}

inline bool FKLDebugImGuiClientServerCacheConnection::TickChild()
{
    return mState == EClientState::Failure;
}
