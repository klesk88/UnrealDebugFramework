// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientServerCacheConnection.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"

// modules
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Engine/World.h"
#include "Serialization/Archive.h"

FKLDebugImGuiClientServerCacheConnection::FKLDebugImGuiClientServerCacheConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket)
    : FKLDebugNetworkingCachedConnectionBase(_ReadBufferSize, _WriteBufferSize, _Socket)
{
}

bool FKLDebugImGuiClientServerCacheConnection::IsValid() const
{
    if (!FKLDebugNetworkingCachedConnectionBase::IsValid())
    {
        return false;
    }

    const ESocketConnectionState SocketConnectionState = GetSocketMutable().GetConnectionState();
    switch (mState)
    {
    case EClientState::FullyInitialized:
        return true;
    case EClientState::Failure:
        return false;
    }

    return false;
}

void FKLDebugImGuiClientServerCacheConnection::TickChildWriteBuffer(FArchive& _Writer)
{
    switch (mState)
    {
    case EClientState::FullyInitialized:
        mClientImGuiData.Parallel_TickWriteData(_Writer);
        break;
    case EClientState::Failure:
        break;
    }
}

bool FKLDebugImGuiClientServerCacheConnection::TickChild()
{
    switch (mState)
    {
    case EClientState::FullyInitialized:
        break;
    case EClientState::Failure:
        break;
    }

    return mState == EClientState::Failure;
}

bool FKLDebugImGuiClientServerCacheConnection::TickOnGameThread(const UWorld& _World, FKLDebugImGuiClientData& _ClientData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiTCPClientCachedConnection_TickOnGameThread);

    TArray<uint8>& WriteBuffer = GetWriteBuffer();
    FKLDebugNetworkingConnectionGetPendingBuffer PendingMessagesContainer(GetReadBuffer());
    TArray<FKLDebugNetworkingPendingMessage>& ReadBuffer = PendingMessagesContainer.GetPendingMessages();

    mClientImGuiData.GameThread_TickReadData(_ClientData, ReadBuffer);
    return HasNewReadData();
}