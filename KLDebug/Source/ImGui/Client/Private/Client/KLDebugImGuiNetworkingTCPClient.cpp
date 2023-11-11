#include "Client/KLDebugImGuiNetworkingTCPClient.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"
#include "Subsystem/Engine/KLDebugImGuiClientSubsystem_Engine.h"

//modules
#include "ImGui/Networking/Public/Message/Discovery/KLDebugImGuiNetworkingMessage_ServerInitializeClientConnection.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Common/TcpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"
#include "Misc/ScopeLock.h"
#include "Misc/ScopeTryLock.h"
#include "SocketSubsystem.h"

void FKLDebugImGuiNetworkingTCPClient::CreateSocket()
{
    InitSocket();
    mCachedConnections.Reserve(3);
}

void FKLDebugImGuiNetworkingTCPClient::InitSocket()
{
    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

    const FIPv4Address IPAddr(127, 0, 0, 1);
    const FIPv4Endpoint Endpoint(IPAddr, Settings.Client_GetPort());

    mListenerSocket = FTcpSocketBuilder(TEXT("ClientListenerSocket"))
        .AsReusable()
        .AsNonBlocking()
        .BoundToEndpoint(Endpoint)
        .Listening(static_cast<int32>(Settings.Client_GetMaxConnectionBacklog()))
        .Build();

    if (!mListenerSocket)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("FKLDebugImGuiNetworkingTCPClient::InitSocket>> Failed in creating listener socket"));
        return;
    }

    int32 ReadNewSize = 0;
    int32 WriteNewSize = 0;
    mListenerSocket->SetReceiveBufferSize(Settings.Client_GetReadBufferSize(), ReadNewSize);
    mListenerSocket->SetSendBufferSize(Settings.Client_GetWriteBufferSize(), WriteNewSize);
}

void FKLDebugImGuiNetworkingTCPClient::RemoveCachedConnection(const int32 _Index)
{
    FKLDebugImGuiTCPClientCachedConnection& CachedConnection = mCachedConnections[_Index];
    CachedConnection.Shutdown();
    mCachedConnections.RemoveAtSwap(_Index, 1, false);
}

void FKLDebugImGuiNetworkingTCPClient::RunChild()
{
    if (mListenerSocket)
    {
        TickPendingConnections();
    }

    TickConnections();
}

void FKLDebugImGuiNetworkingTCPClient::TickPendingConnections()
{
    //should be safe to do this in parallel as FTcpMessageTransportConnection::Run does the same
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPClient::TickPendingConnections>> Could not get socket subsystem"));
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

    bool Pending = false;

    // handle incoming connections
    if (mListenerSocket->HasPendingConnection(Pending) && Pending)
    {
        //New Connection receive!
        FSocket* ConnectionSocket = mListenerSocket->Accept(TEXT("Server Socket"));
        if (!ConnectionSocket)
        {
            UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiNetworkingTCPClient::TickPendingConnections>> Listen socket received a pending connection event but ListenSocket->Accept failed to create a ClientSocket."));
            return;
        }

        int32 ReadNewSize = 0;
        int32 WriteNewSize = 0;
        ConnectionSocket->SetNonBlocking(true);
        ConnectionSocket->SetReceiveBufferSize(Settings.Client_GetReadBufferSize(), ReadNewSize);
        ConnectionSocket->SetSendBufferSize(Settings.Client_GetWriteBufferSize(), WriteNewSize);

        mCachedConnections.Emplace(ReadNewSize, WriteNewSize, *ConnectionSocket);

#if !WITH_EDITOR
        //ok so in a package build we have one of this class per instance. So in that case, as soon as we connect to the server we want to free the socket
        //for other clients launched on the same machine that want to connect with it.
        //However in editor we have one single instance for all clients we generate so we never want to close this socket
        mListenerSocket->Close();
        SocketSubsystem->DestroySocket(mListenerSocket);
        mListenerSocket = nullptr;
#endif
    }
}

void FKLDebugImGuiNetworkingTCPClient::TickConnections()
{
    bool RequiresGameThreadTick = false;
    bool HasServerInitializedOwner = false;

    for (int32 i = mCachedConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiTCPClientCachedConnection& CachedConnection = mCachedConnections[i];
        if (!CachedConnection.IsValid())
        {
            RemoveCachedConnection(i);
            continue;
        }

        HasServerInitializedOwner |= CachedConnection.HasServerInitializedOwner();
        CachedConnection.Tick();
        RequiresGameThreadTick |= CachedConnection.HasNewReadData();
    }

    //this works without locks because we are sure the subsystem exists because it is owning us
    if (RequiresGameThreadTick || HasServerInitializedOwner)
    {
        UKLDebugImGuiClientSubsystem_Engine* EngineSubsystem = UKLDebugImGuiClientSubsystem_Engine::TryGetMutable();
        if (RequiresGameThreadTick)
        {
            EngineSubsystem->SetShouldTick();
        }
        if (HasServerInitializedOwner)
        {
            EngineSubsystem->SetHasServerInitializedOwner(HasServerInitializedOwner);
        }
    }
}

void FKLDebugImGuiNetworkingTCPClient::TickGameThread(FKLDebugImGuiClientGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPClient_TickGameThread);

    //perform the update from the game thread. Here we are sure we can access actors and other systems without possibly causing
    //crashes due to lock or other things

    FScopeTryLock TryLock(&mGameThreadUpdateLock);
    if (!TryLock.IsLocked())
    {
        _Context.SetShouldKeepTicking(true);
        return;
    }

    if (mCachedConnections.IsEmpty())
    {
        _Context.SetShouldKeepTicking(true);
        return;
    }

    GameThread_RemoveInvalidWorlds(_Context);
    GameThread_TickImGuiData(_Context);
}

void FKLDebugImGuiNetworkingTCPClient::GameThread_RemoveInvalidWorlds(const FKLDebugImGuiClientGameThreadContext& _Context)
{
    for (const FObjectKey& RemovedWorld : _Context.GetRemovedWorlds())
    {
        for (int32 i = mCachedConnections.Num() - 1; i >= 0; --i)
        {
            const FKLDebugImGuiTCPClientCachedConnection& ConnectionData = mCachedConnections[i];
            if (ConnectionData.IsPartOfWorld(RemovedWorld))
            {
                RemoveCachedConnection(i);
            }
        }
    }
}

void FKLDebugImGuiNetworkingTCPClient::GameThread_TickImGuiData(FKLDebugImGuiClientGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPClient_TickImGuiData);

    bool KeepTicking = false;
    for (FKLDebugImGuiClientData& ClientData : _Context.GetClientDataMutable())
    {
        const FNetworkGUID PlayerID = ClientData.GetLocalPlayerNetworkID();
        if (!PlayerID.IsValid())
        {
            KeepTicking |= true;
            continue;
        }

        FKLDebugImGuiTCPClientCachedConnection* CachedConnection = mCachedConnections.FindByKey(PlayerID);
        if (!CachedConnection)
        {
            KeepTicking |= true;
            continue;
        }

        KeepTicking |= CachedConnection->TickOnGameThread(ClientData);
    }

    _Context.SetShouldKeepTicking(KeepTicking);
}

