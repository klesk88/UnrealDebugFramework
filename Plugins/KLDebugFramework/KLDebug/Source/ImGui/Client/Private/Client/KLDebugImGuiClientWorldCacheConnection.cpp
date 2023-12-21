// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientWorldCacheConnection.h"

// engine
#include "Engine/World.h"

FKLDebugImGuiClientWorldCacheConnection::FKLDebugImGuiClientWorldCacheConnection(const FObjectKey& _WorldKey, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _WorldServerSocket)
    : mCacheConnection(_ReadBufferSize, _WriteBufferSize, _WorldServerSocket)
    , mWorldKey(_WorldKey)
{
}

bool FKLDebugImGuiClientWorldCacheConnection::Parallel_Tick()
{
    mCacheConnection.Tick();
    return mCacheConnection.HasNewReadData();
}

bool FKLDebugImGuiClientWorldCacheConnection::TickOnGameThread(FKLDebugImGuiClientData& _ClientData)
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiClientWorldCacheConnection_TickOnGameThread);

    const UWorld* World = Cast<const UWorld>(mWorldKey.ResolveObjectPtr());
    if (!World)
    {
        ensureMsgf(false, TEXT("world should be valid"));
        return false;
    }

    return mCacheConnection.TickOnGameThread(*World, _ClientData);
}
