#include "Server/KLDebugImGuiNetworkingTCPServer.h"

#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"
#include "Subsystem/KLDebugImGuiServerSubsystem_Engine.h"

//modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Common/TcpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"
#include "Misc/ScopeTryLock.h"
#include "SocketSubsystem.h"

void FKLDebugImGuiNetworkingTCPServer::CreateSocket()
{
    InitSocket();
}

void FKLDebugImGuiNetworkingTCPServer::InitSocket()
{
    const FIPv4Address IPAddr(127, 0, 0, 1);
    
    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    uint32 ServerPort = Settings.Server_GetStartPortSearch();
    const uint32 MaxClientPortDiscoveryTries = Settings.Server_GetMaxPortDiscoveryRetries();
    uint32 CurrentRetry = 0;
    
    while (!mListenerSocket && CurrentRetry < MaxClientPortDiscoveryTries)
    {
        const FIPv4Endpoint Endpoint(IPAddr, ServerPort++);

        mListenerSocket = FTcpSocketBuilder(TEXT("ServerSocket"))
            .AsReusable()
            .AsNonBlocking()
            .BoundToEndpoint(Endpoint)
            .Listening(Settings.Server_GetMaxConnectionBacklog())
            .Build();
    }

    if (!mListenerSocket)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("FKLDebugImGuiNetworkingTCPServer::InitSocket>> Failed in creating listener socket"));
        return;
    }

    int32 NewSize = 0;
    mListenerSocket->SetReceiveBufferSize(Settings.Server_GetReadBufferSize(), NewSize);
    mListenerSocket->SetSendBufferSize(Settings.Server_GetWriteBufferSize(), NewSize);
}

void FKLDebugImGuiNetworkingTCPServer::RemoveCachedConnection(const int32 _Index)
{
    FKLDebugImGuiTCPServerCachedConnection& RemoveConnection = mClientsCachedConnection[_Index];
    RemoveConnection.Shutdown();
    mClientsCachedConnection.RemoveAtSwap(_Index, 1, false);
}

void FKLDebugImGuiNetworkingTCPServer::RunChild()
{
    if (!mListenerSocket)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("FKLDebugImGuiNetworkingTCPServer::RunChild>> Socket has not been created. Stop server"));
        Stop();
        return;
    }

    TickRemovePlayerConnections();
    TickCachedConnections();
}

void FKLDebugImGuiNetworkingTCPServer::TickRemovePlayerConnections()
{
    for (int32 i = mClientsCachedConnection.Num() - 1; i >= 0; --i)
    {
        const FKLDebugImGuiTCPServerCachedConnection& ConnectionData = mClientsCachedConnection[i];
        if (!ConnectionData.IsValidConnection())
        {
            RemoveCachedConnection(i);
        }
    }
}

void FKLDebugImGuiNetworkingTCPServer::TickCachedConnections()
{
    bool RequiresGameThreadTick = false;
    for (int32 i = mClientsCachedConnection.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiTCPServerCachedConnection& CachedConnection = mClientsCachedConnection[i];
        if (!CachedConnection.IsValid())
        {
            RemoveCachedConnection(i);
            continue;
        }

        CachedConnection.Tick();
        RequiresGameThreadTick |= CachedConnection.HasNewReadData();
    }

    //this works without locks because we are owned by the KLDebugImGuiServerSubsystem_Engine
    //so we are sure the object is valid
    if (RequiresGameThreadTick)
    {
        UKLDebugImGuiServerSubsystem_Engine* EngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
        EngineSubsystem->SetShouldTick();
    }
}

void FKLDebugImGuiNetworkingTCPServer::TickGameThread(FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPServer_TickGameThread);

    //perform the update from the game thread. Here we are sure we can access actors and other systems without possibly causing
    //crashes due to lock or other things

    FScopeTryLock TryLock(&mGameThreadUpdateLock);
    if (!TryLock.IsLocked())
    {
        //retry next frame
        _Context.SetShouldKeepTicking(true);
        return;
    }

    GameThread_RemoveOldWorlds(_Context);
    GameThread_UpdateNewPlayersConnections(_Context);
    GameThread_RemovePlayersConnection(_Context);
    const bool KeepTicking = GameThread_UpdateConnections(_Context);
    _Context.SetShouldKeepTicking(KeepTicking);
}

void FKLDebugImGuiNetworkingTCPServer::GameThread_RemoveOldWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    for (const FObjectKey& RemovedWorld : _Context.GetRemovedWorlds())
    {
        for (int32 i = mClientsCachedConnection.Num() - 1; i >= 0; --i)
        {
            const FKLDebugImGuiTCPServerCachedConnection& ConnectionData = mClientsCachedConnection[i];
            if (ConnectionData.IsPartOfWorld(RemovedWorld))
            {
                RemoveCachedConnection(i);
            }
        }
    }
}

void FKLDebugImGuiNetworkingTCPServer::GameThread_UpdateNewPlayersConnections(FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPServer_UpdateNewPlayersConnections);

    static const FString SocketDescription(TEXT("KLDebugImGuiServer_NewClientSocket"));
    //should be safe to do this in parallel as FTcpMessageTransportConnection::Run does the same
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::TickNewPlayerConnections>> Could not get socket subsystem"));
        return;
    }

    TArray<TWeakObjectPtr<const APlayerController>>& NewPlayers = _Context.GetNewConnectedPlayers();
    if (NewPlayers.IsEmpty())
    {
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    const uint32 ClientPort = Settings.Client_GetPort();

    for (const TWeakObjectPtr<const APlayerController>& NewPlayerController : NewPlayers)
    {
        if (!NewPlayerController.IsValid())
        {
            continue;
        }

        const FObjectKey ControllerKey(NewPlayerController.Get());
        ensureMsgf(mClientsCachedConnection.FindByKey(ControllerKey) == nullptr, TEXT("controller is already in connected list not expected"));

        const UNetConnection* NetConnection = NewPlayerController->GetNetConnection();
        if (!NetConnection)
        {
            ensureMsgf(false, TEXT("No valid net connection for new player. Not expected"));
            continue;
        }

        const FString& HostString = NetConnection->URL.Host;

        bool IsValid = false;
        TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();
        Address->SetIp(*HostString, IsValid);
        if (!IsValid)
        {
            UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::TickNewPlayerConnections>> Invalid Ip Address [%s]"), *HostString);
            continue;
        }

        Address->SetPort(ClientPort);
        FSocket* NewClientSocket = FTcpSocketBuilder(SocketDescription)
            .AsNonBlocking()
            .AsReusable()
            .Build();

        if (!NewClientSocket)
        {
            UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::TickNewPlayerConnections>> Failed to create socket"));
            continue;
        }

        int32 ReceiveBufferSize = 0;
        int32 SenderBufferSize = 0;
        NewClientSocket->SetReceiveBufferSize(Settings.Client_GetReadBufferSize(), ReceiveBufferSize);
        NewClientSocket->SetSendBufferSize(Settings.Client_GetWriteBufferSize(), SenderBufferSize);

        NewClientSocket->Connect(*Address);

        const KL::Debug::ImGui::Networking::Server::CacheConnectionID NewID = (mCacheConnectionsID + 1) % TNumericLimits<KL::Debug::ImGui::Networking::Server::CacheConnectionID>::Max();
        mClientsCachedConnection.Emplace(*NewPlayerController.Get(), NewID, ReceiveBufferSize, SenderBufferSize, *NewClientSocket);
    }

    NewPlayers.Reset();
}

void FKLDebugImGuiNetworkingTCPServer::GameThread_RemovePlayersConnection(FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    TArray<FObjectKey>& NewPlayers = _Context.GetDisconnectedPlayers();
    if (NewPlayers.IsEmpty())
    {
        return;
    }

    for (const FObjectKey& RemovedPlayer : NewPlayers)
    {
        if (FKLDebugImGuiTCPServerCachedConnection* Connection = mClientsCachedConnection.FindByKey(RemovedPlayer))
        {
            Connection->SetInvalidConnection();
        }
    }

    NewPlayers.Reset();
}

bool FKLDebugImGuiNetworkingTCPServer::GameThread_UpdateConnections(const FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPServer_GTUpdateConnections);

    bool KeepTicking = false;
    for (FKLDebugImGuiTCPServerCachedConnection& ClientConnection : mClientsCachedConnection)
    {
        if (!ClientConnection.IsValidConnection())
        {
            //pending to be removed
            continue;
        }

        //if any of the connection want to keep ticking then keep ticking next frame
        KeepTicking |= ClientConnection.TickOnGameThread();
    }

    return KeepTicking;
}

