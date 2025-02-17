// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiNetworkingTCPServer.h"

#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"
#include "Subsystem/Engine/KLDebugImGuiServerSubsystem_Engine.h"

// modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "User/Framework/Internal/Networking/Server/KLDebugNetworkingServerDelegatesInternal.h"
#include "User/Framework/Public/Networking/Server/KLDebugNetworkingServerDelegates.h"

// engine
#include "Common/TcpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "Engine/NetDriver.h"
#include "Engine/World.h"
#include "Misc/ScopeTryLock.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

void FKLDebugImGuiNetworkingTCPServer::RemoveCachedConnection(const int32 _Index)
{
    if (mArbitrerManager.HasBeenInitialized())
    {
        FKLDebugImGuiServerWorldCachedConnection& RemoveConnection = mWorldCachedConnections[_Index];
        mArbitrerManager.ArbitrerRemovedWorldConnection(RemoveConnection.GetServerPort());
    }

    mWorldCachedConnections.RemoveAtSwap(_Index, 1, EAllowShrinking::No);
}

void FKLDebugImGuiNetworkingTCPServer::RunChild()
{
    Parallel_TickCachedConnections();
    Parallel_TickArbitrerConnection();
}

void FKLDebugImGuiNetworkingTCPServer::Exit()
{
    FKLDebugImGuiNetworkingTCPBase::Exit();

    mArbitrerManager.Shutdown();
}

void FKLDebugImGuiNetworkingTCPServer::Parallel_TickCachedConnections()
{
    if (mWorldCachedConnections.IsEmpty())
    {
        return;
    }

    bool RequiresGameThreadTick = false;
    for (FKLDebugImGuiServerWorldCachedConnection& WorldConnection : mWorldCachedConnections)
    {
        RequiresGameThreadTick |= WorldConnection.Parallel_Tick();
    }

    // this works without locks because we are owned by the KLDebugImGuiServerSubsystem_Engine
    // so we are sure the object is valid
    if (RequiresGameThreadTick)
    {
        UKLDebugImGuiServerSubsystem_Engine* EngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
        EngineSubsystem->SetShouldTick();
    }
}

void FKLDebugImGuiNetworkingTCPServer::Parallel_TickArbitrerConnection()
{
    if (mArbitrerManager.HasBeenInitialized())
    {
        mArbitrerManager.Parallel_Tick();
    }
}

void FKLDebugImGuiNetworkingTCPServer::TickGameThread(FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPServer_TickGameThread);

    // perform the update from the game thread. Here we are sure we can access actors and other systems without possibly causing
    // crashes due to lock or other things

    FScopeTryLock TryLock(&mGameThreadUpdateLock);
    if (!TryLock.IsLocked())
    {
        // retry next frame
        _Context.SetShouldKeepTicking(true);
        return;
    }

    GameThread_AddNewWorlds(_Context);
    GameThread_RemoveOldWorlds(_Context);
    const bool KeepTicking = GameThread_UpdateConnections(_Context);
    _Context.SetShouldKeepTicking(KeepTicking);
}

void FKLDebugImGuiNetworkingTCPServer::GameThread_RemoveOldWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    for (const FObjectKey& RemovedWorld : _Context.GetRemovedWorlds())
    {
        const int32 Index = mWorldCachedConnections.IndexOfByKey(RemovedWorld);
        if (Index == INDEX_NONE)
        {
            ensureMsgf(false, TEXT("should not happen"));
            continue;
        }

        RemoveCachedConnection(Index);
    }
}

void FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    if (_Context.GetNewWorlds().IsEmpty())
    {
        return;
    }

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> Could not get socket subsystem"));
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    FString LocalPort;
    int32 DebugPort = 0;

    uint32 StartWorldPort = Settings.Server_GetStartPortRange();
    uint32 EndWorldPort = Settings.Server_GetEndPortRange();
    const TOptional<KL::Debug::Server::Delegates::FServerSocketPortRangeDelegateData> OverridePortsData = KL::Debug::Server::Delegates::Internal::BroadcastOnGetDebugServerSocketPortRange();
    if (OverridePortsData.IsSet())
    {
        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> Overriding debug ports range new start [%u] new end [%u]"), StartWorldPort, EndWorldPort);

        StartWorldPort = OverridePortsData.GetValue().GetStartRange();
        EndWorldPort = OverridePortsData.GetValue().GetEndRange();
    }

    for (const TWeakObjectPtr<const UWorld>& AddeddWorld : _Context.GetNewWorlds())
    {
        if (!AddeddWorld.IsValid())
        {
            continue;
        }

        const TSharedPtr<FInternetAddr> WorldLocalAddress = GetWorldLocalAddress(*AddeddWorld.Get());
        if (!WorldLocalAddress.IsValid())
        {
            ensureMsgf(false, TEXT("No valid net connection for world. Not expected"));
            continue;
        }

        FSocket* WorldDebugSocket = GetNewWorldSocket(StartWorldPort, EndWorldPort, Settings, DebugPort);
        if (!WorldDebugSocket)
        {
            UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> Failed in creating arbitrer socket for world [%s]"), *AddeddWorld->GetName());
            continue;
        }

        const int32 WorldServerPort = WorldLocalAddress->GetPort();
        int32 ReceiveBufferSize = 0;
        int32 SenderBufferSize = 0;
        WorldDebugSocket->SetReceiveBufferSize(static_cast<int32>(Settings.Server_GetReadBufferSize()), ReceiveBufferSize);
        WorldDebugSocket->SetSendBufferSize(static_cast<int32>(Settings.Server_GetWriteBufferSize()), SenderBufferSize);
        mWorldCachedConnections.Emplace(*AddeddWorld.Get(), WorldServerPort, DebugPort, *WorldDebugSocket);

        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> New World [%s] connection added with server port [%d] and debug port [%d]"), *AddeddWorld->GetName(), WorldServerPort, DebugPort);

        if (mArbitrerManager.HasBeenInitialized())
        {
            mArbitrerManager.ArbitrerAddWorldConnection(WorldServerPort, DebugPort);
        }
    }
}

bool FKLDebugImGuiNetworkingTCPServer::GameThread_UpdateConnections(const FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPServer_GTUpdateConnections);

    bool KeepTicking = false;
    for (FKLDebugImGuiServerWorldCachedConnection& ClientConnection : mWorldCachedConnections)
    {
        // if any of the connection want to keep ticking then keep ticking next frame
        KeepTicking |= ClientConnection.TickOnGameThread();
    }

    return KeepTicking;
}

FSocket* FKLDebugImGuiNetworkingTCPServer::GetNewWorldSocket(const uint32 _StartPort, const uint32 _EndPort, const UKLDebugImGuiNetworkingSettings& _Settings, int32& _DebugPort) const
{
    uint32 CurrentPort = _StartPort;
    FSocket* NewSocket = nullptr;
    while (!NewSocket && CurrentPort <= _EndPort)
    {
        _DebugPort = CurrentPort++;
        const FIPv4Endpoint Endpoint(FIPv4Address::Any, static_cast<int32>(_DebugPort));
        NewSocket = FTcpSocketBuilder(TEXT("ClientListenerSocket"))
                        .AsNonBlocking()
                        .BoundToEndpoint(Endpoint)
                        .Listening(static_cast<int32>(_Settings.Server_GetMaxConnectionBacklog()))
                        .Build();
    }

    if (!NewSocket)
    {
        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiNetworkingTCPServer::GetNewWorldSocket>> Failed in creating socket"));
        return nullptr;
    }

    return NewSocket;
}

const TSharedPtr<FInternetAddr> FKLDebugImGuiNetworkingTCPServer::GetWorldLocalAddress(const UWorld& _World) const
{
    const UNetDriver* WorldNetDriver = _World.GetNetDriver();
    return WorldNetDriver ? WorldNetDriver->LocalAddr : nullptr;
}
