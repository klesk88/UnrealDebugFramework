// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Server/KLDebugImGuiServerCacheConnection.h"

// modules
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingCachedConnectionBase.h"

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"

class FSocket;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerClientCachedConnection final : public FKLDebugNetworkingCachedConnectionBase
{
public:
    explicit FKLDebugImGuiServerClientCachedConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _ClientSocket);

    // this is called from the game thread.
    UE_NODISCARD bool TickOnGameThread(const UWorld& _World);

private:
    FKLDebugImGuiServerCacheConnection mClientDataForConnection;
    double mCheckTimer;
};
