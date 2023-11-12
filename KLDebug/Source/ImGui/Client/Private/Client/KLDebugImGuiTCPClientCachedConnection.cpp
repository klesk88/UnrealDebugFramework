#include "Client/KLDebugImGuiTCPClientCachedConnection.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"
#include "Message/Header/KLDebugImGuiNetworkingMessage_Header.h"

//modules
#include "ImGui/Networking/Public/Message/Discovery/KLDebugImGuiNetworkingMessage_ServerInitializeClientConnection.h"
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Misc/ScopeLock.h"
#include "Misc/ScopeTryLock.h"
#include "Serialization/Archive.h"
#include "Sockets.h"

FKLDebugImGuiTCPClientCachedConnection::FKLDebugImGuiTCPClientCachedConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket)
    : FKLDebugImGuiNetworkingTCPCachedConnectionBase(_ReadBufferSize, _WriteBufferSize, _Socket)
{
    mCheckTimer = FDateTime::Now();
}

bool FKLDebugImGuiTCPClientCachedConnection::IsValid() const
{
    if (!FKLDebugImGuiNetworkingTCPCachedConnectionBase::IsValid())
    {
        return false;
    }

    const ESocketConnectionState SocketConnectionState = GetSocketMutable().GetConnectionState();
    switch (mState)
    {
    case EClientState::PendingServerData:
        return true;
    case EClientState::FullyInitialized:
        return SocketConnectionState == SCS_Connected;
    case EClientState::Failure:
        return false;
    }

    return false;
}

bool FKLDebugImGuiTCPClientCachedConnection::ReadBufferChildHasHandleMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, FArchive& _Data)
{
    switch (mState)
    {
    case EClientState::PendingServerData:
        return PendingServerDataStateReadData(_Header, _Data);
    case EClientState::FullyInitialized:
    case EClientState::Failure:
        break;
    }

    return false;
}

void FKLDebugImGuiTCPClientCachedConnection::TickChildWriteBuffer(FArchive& _Writer)
{
    switch (mState)
    {
    case EClientState::FullyInitialized:
        mClientImGuiData.Parallel_TickWriteData(_Writer);
        break;
    case EClientState::PendingServerData:
    case EClientState::Failure:
        break;
    }
}

bool FKLDebugImGuiTCPClientCachedConnection::PendingServerDataStateReadData(const FKLDebugImGuiNetworkingMessage_Header& _Header, FArchive& _Data)
{
    if (static_cast<EKLDebugNetworkMessageTypes>(_Header.GetMessageType()) != EKLDebugNetworkMessageTypes::Server_InitClientConnection)
    {
        UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiTCPClientCachedConnection::PendingServerDataStateReadData>> Data receive contained an unexpected message. Refuse"));
        mState = EClientState::Failure;
        return false;
    }

    const FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection Message{ _Data };
    if (!Message.IsValid())
    {
        UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiTCPClientCachedConnection::PendingServerDataStateReadData>> We are not in sync with the server. Drop teh connection"));
        mState = EClientState::Failure;
        return false;
    }

    mControllerOwnerGUID = Message.Client_GetPlayerController();
    mState = EClientState::FullyInitialized;
    return true;
}

bool FKLDebugImGuiTCPClientCachedConnection::TickChild()
{
    switch (mState)
    {
    case EClientState::PendingServerData:
        HandlePendingServerDataState();
        break;
    case EClientState::FullyInitialized:
    case EClientState::Failure:
        break;
    }

    return mState == EClientState::Failure;
}

void FKLDebugImGuiTCPClientCachedConnection::HandlePendingServerDataState()
{
    const FTimespan TimeSpan = FDateTime::Now() - mCheckTimer;
    if (TimeSpan.GetTotalSeconds() > 4.f)
    {
        UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiTCPClientCachedConnection::HandlePendingServerDataState>> Too much time waiting"));
        mState = EClientState::Failure;
    }
}

bool FKLDebugImGuiTCPClientCachedConnection::TickOnGameThread(FKLDebugImGuiClientData& _ClientData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiTCPClientCachedConnection_TickOnGameThread);

    //when we reach here we have locked so any other method will be called
    if (mWorldKey == FObjectKey())
    {
        mWorldKey = _ClientData.GetWorldID();
    }
    else
    {
        ensureMsgf(mWorldKey == _ClientData.GetWorldID(), TEXT("must be the same"));
    }

    TArray<uint8>& WriteBuffer = GetWriteBuffer();
    FKLDebugImGuiNetworkingConnectionGetPendingBuffer PendingMessagesContainer(GetReadBuffer());
    TArray<FKLDebugImGuiNetworkingPendingMessage>& ReadBuffer = PendingMessagesContainer.GetPendingMessages();

    mClientImGuiData.GameThread_TickReadData(_ClientData, ReadBuffer);
    return HasNewReadData();
}