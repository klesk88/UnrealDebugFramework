// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientManager.h"

// modules
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingCachedConnectionBase.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"

class FArchive;
class FKLDebugImGuiClientData;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientServerCacheConnection final : public FKLDebugNetworkingCachedConnectionBase
{
private:
    enum class EClientState : uint8
    {
        FullyInitialized = 0,

        Failure
    };

public:
    explicit FKLDebugImGuiClientServerCacheConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket);

    // FKLDebugImGuiNetworkingTCPCachedConnectionBase
    UE_NODISCARD bool IsValid() const final;
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase

    UE_NODISCARD bool TickOnGameThread(const UWorld& _World, FKLDebugImGuiClientData& _ClientData);

private:
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase
    UE_NODISCARD bool TickChild() final;
    void TickChildWriteBuffer(FArchive& _Writer) final;
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase

private:
    FKLDebugImGuiClientManager mClientImGuiData;
    EClientState mState = EClientState::FullyInitialized;
    bool mIsWorldValid = true;
};
