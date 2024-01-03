// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientWorldCacheConnection.h"

// engine
#include "Engine/World.h"

FKLDebugImGuiClientWorldCacheConnection::FKLDebugImGuiClientWorldCacheConnection(const FObjectKey& _WorldKey, const FUniqueNetIdRepl& _LocalPlayerNetID, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _WorldServerSocket)
    : mCacheConnection(_WorldKey, _LocalPlayerNetID, _ReadBufferSize, _WriteBufferSize, _WorldServerSocket)
    , mWorldKey(_WorldKey)
{
}

bool FKLDebugImGuiClientWorldCacheConnection::Parallel_Tick()
{
    mCacheConnection.Tick();
    return mCacheConnection.HasPendingDataToRead();
}

bool FKLDebugImGuiClientWorldCacheConnection::TickOnGameThread(FKLDebugImGuiClientData& _ClientData)
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiClientWorldCacheConnection_TickOnGameThread);

    UWorld* World = Cast<UWorld>(mWorldKey.ResolveObjectPtr());
    if (!World)
    {
        ensureMsgf(false, TEXT("world should be valid"));
        return false;
    }

    return mCacheConnection.TickOnGameThread(*World, _ClientData);
}
