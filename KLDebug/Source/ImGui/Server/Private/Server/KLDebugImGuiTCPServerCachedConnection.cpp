#include "Server/KLDebugImGuiTCPServerCachedConnection.h"

//modules
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Networking/Public/Message/Discovery/KLDebugImGuiNetworkingMessage_ServerInitializeClientConnection.h"
#include "ImGui/Networking/Public/Message/Helpers/KLDebugImGuiNetworkingMessage_Helpers.h"
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiTCPServerCachedConnection::FKLDebugImGuiTCPServerCachedConnection(const AController& _OwnerController, const KL::Debug::ImGui::Networking::Server::CacheConnectionID _ID, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _ClientSocket)
    : FKLDebugImGuiNetworkingTCPCachedConnectionBase(_ReadBufferSize, _WriteBufferSize, _ClientSocket)
    , mClientDataForConnection()
    , mControllerKey(&_OwnerController)
{
    check(IsInGameThread());

    mWorld = _OwnerController.GetWorld();
    ensureMsgf(mWorld.IsValid(), TEXT("must have a valid world here"));
    mWorldKey = mWorld.Get();
    if (mWorld.IsValid())
    {
        mWorldNetworkKey = KL::Debug::Networking::Helpers::TryGetNetworkGuid(*mWorld.Get());
    }

    mControllerNetworkKey = KL::Debug::Networking::Helpers::TryGetNetworkGuid(_OwnerController);
}

bool FKLDebugImGuiTCPServerCachedConnection::IsValidConnection() const
{
    if (!IsValid() || !mIsValidConnection)
    {
        return false;
    }

    const ESocketConnectionState SocketState = GetSocketMutable().GetConnectionState();
    switch (mConnectionState)
    {
    case EConnectionState::ToInitialize:
    case EConnectionState::PendingInitializeSend:
    case EConnectionState::PendingClientConnection:
        return true;
    case EConnectionState::ClientConnected:
        return SocketState == SCS_Connected;
    case EConnectionState::Failure:
        return false;
    }

    return false;
}

bool FKLDebugImGuiTCPServerCachedConnection::TickChild()
{
    switch (mConnectionState)
    {
    case EConnectionState::ToInitialize:
        HandleToInitializeState();
        break;
    case EConnectionState::PendingInitializeSend:
        break;
    case EConnectionState::PendingClientConnection:
        HandlePendingClientConnectionState();
        break;
    case EConnectionState::ClientConnected:
        HandleClientConnectedState();
        break;
    case EConnectionState::Failure:
        break;
    }

    return mConnectionState == EConnectionState::Failure;
}

void FKLDebugImGuiTCPServerCachedConnection::HandleToInitializeState()
{
}

void FKLDebugImGuiTCPServerCachedConnection::HandlePendingClientConnectionState()
{
    const ESocketConnectionState SocketState = GetSocketMutable().GetConnectionState();
    if (SocketState == SCS_Connected)
    {
        mConnectionState = EConnectionState::ClientConnected;
        return;
    }

    const FTimespan TimeSpan = mCheckTimer - FDateTime::Now();
    if (TimeSpan.GetTotalSeconds() > 10.f)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiTCPServerCachedConnection::HandlePendingClientConnectionState>> waited for 10 seconds but client didnt connect"));
        mConnectionState = EConnectionState::Failure;
    }
}

void FKLDebugImGuiTCPServerCachedConnection::HandleClientConnectedState()
{
}

void FKLDebugImGuiTCPServerCachedConnection::TickChildWriteBuffer(FArchive& _Writer)
{
    switch (mConnectionState)
    {
    case EConnectionState::ToInitialize:
        WriteInitializeState(_Writer);
        break;
    case EConnectionState::PendingInitializeSend:
    case EConnectionState::PendingClientConnection:
    case EConnectionState::ClientConnected:
    case EConnectionState::Failure:
        break;
    }
}

void FKLDebugImGuiTCPServerCachedConnection::WriteInitializeState(FArchive& _Writer)
{
    //here we are in parallel. Access the cached information from the game thread
    if (!mWorldNetworkKey.IsValid() || !mControllerNetworkKey.IsValid())
    {
        ensureMsgf(false, TEXT("we should have valid network IDS at this point as well"));
        mConnectionState = EConnectionState::Failure;
        return;
    }

    TArray<uint8> TempData;
    TempData.Reserve(40);
    FMemoryWriter WriterTempData(TempData);
    FKLDebugImGuiNetworkingMessage_ServerInitializeClientConnection InitializeConnection{ mWorldNetworkKey, mControllerNetworkKey };
    InitializeConnection.Serialize(WriterTempData);
    KL::Debug::ImGuiNetworking::Message::PrepareMessageToSend_Uncompressed(InitializeConnection, TempData, _Writer);

    mConnectionState = EConnectionState::PendingInitializeSend;
    mCheckTimer = FDateTime::Now();
}

void FKLDebugImGuiTCPServerCachedConnection::OnSendDataResultChild(const bool _DataSentFully)
{
    switch (mConnectionState)
    {
    case EConnectionState::PendingInitializeSend:
        if (_DataSentFully)
        {
            mCheckTimer = FDateTime::Now();
            mConnectionState = EConnectionState::PendingClientConnection;
        }
        else
        {
            const FTimespan TimeSpan = mCheckTimer - FDateTime::Now();
            if (TimeSpan.GetTotalSeconds() > 5.f)
            {
                UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiTCPServerCachedConnection::OnSendDataResultChild>> we could not send any bytes"));
                mConnectionState = EConnectionState::Failure;
            }
        }
        break;
    case EConnectionState::ToInitialize:
    case EConnectionState::PendingClientConnection:
    case EConnectionState::ClientConnected:
    case EConnectionState::Failure:
        break;
    }
}

bool FKLDebugImGuiTCPServerCachedConnection::TickOnGameThread()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiTCPServerCachedConnection_TickOnGameThread);

    //NOTE: when we are inside this call, we are sure that no other operation is performed in this connection as we are locking
    //outside of it
    const UWorld* World = mWorld.Get();
    if (!World)
    {
        return false;
    }

    const UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    const FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainer = ImGuiEngineSubsystem->GetFeatureContainerManager();

    TArray<uint8>& WriteBuffer = GetWriteBuffer();
    FKLDebugImGuiNetworkingConnectionGetPendingBuffer PendingMessagesContainer(GetReadBuffer());
    TArray<FKLDebugImGuiNetworkingPendingMessage>& ReadBuffer = PendingMessagesContainer.GetPendingMessages();
    FMemoryWriter Writer{ WriteBuffer };

    mClientDataForConnection.ReadData(*World, FeaturesContainer, ReadBuffer);
    mClientDataForConnection.WriteData(*World, FeaturesContainer, Writer);

    return HasNewReadData() || mClientDataForConnection.NeedsTicking();
}
