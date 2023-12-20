// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiNetworkingTCPServer.h"

#include "Delegates/KLDebugImGuiServerDelegates.h"
#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"
#include "Subsystem/Engine/KLDebugImGuiServerSubsystem_Engine.h"

// modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Arbitrer/Public/Definitions/KLDebugNetworkingArbitrerDefinitions.h"
#include "Networking/Arbitrer/Public/Luncher/KLDebugNetworkingArbitrerLuncher.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerConnected.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerDisconnected.h"
#include "Networking/Arbitrer/Public/Settings/KLDebugNetworkingArbitrerSettings.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Common/TcpSocketBuilder.h"
#include "Common/UdpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "Engine/NetDriver.h"
#include "Engine/World.h"
#include "Misc/ScopeTryLock.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

void FKLDebugImGuiNetworkingTCPServer::RemoveCachedConnection(const int32 _Index, FArchive& _ArbitrerWriter)
{
    if (mArbitrerAddress.Get())
    {
        FKLDebugImGuiServerWorldCachedConnection& RemoveConnection = mWorldCachedConnections[_Index];
        ArbitrerRemovedWorldConnection(RemoveConnection.GetServerPort(), _ArbitrerWriter);
    }

    mWorldCachedConnections.RemoveAtSwap(_Index, 1, false);
}

void FKLDebugImGuiNetworkingTCPServer::RunChild()
{
    Parallel_TickCachedConnections();
    Parallel_TickArbitrerConnection();
}

void FKLDebugImGuiNetworkingTCPServer::Parallel_TickCachedConnections()
{
    if (mWorldCachedConnections.IsEmpty())
    {
        return;
    }

    checkf(mArbitrerAddress.IsValid(), TEXT("Internet address must be valid"));

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
    // keep in mind arbitrer can exist or not

    if (!mListenerSocket || mArbitrerTempBuffer.IsEmpty())
    {
        return;
    }

    if (!mArbitrerAddress.IsValid())
    {
        ensureMsgf(false, TEXT("Arbitrer addr must be valid if socket is valid"));
        return;
    }

    int32 DataSent = 0;
    if (!mListenerSocket->SendTo(mArbitrerTempBuffer.GetData(), mArbitrerTempBuffer.Num(), DataSent, *mArbitrerAddress))
    {
        // we dont expect this. Arbitrer and server machine should live in the same enviroment (same PC for example) so the message
        // should be always be sent correctly.
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::NewWorldConnectionAdded>> Failed tosend arbitrer message."));
    }

    mArbitrerTempBuffer.Reset();
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

    GameThread_InitLocalAddress(_Context);
    GameThread_AddNewWorlds(_Context);
    GameThread_RemoveOldWorlds(_Context);
    const bool KeepTicking = GameThread_UpdateConnections(_Context);
    _Context.SetShouldKeepTicking(KeepTicking);
}

void FKLDebugImGuiNetworkingTCPServer::GameThread_InitLocalAddress(const FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    // Initialize stuff that requires a world or the engine subsystem to have run here.

    if (mLocalAddress.Value != TNumericLimits<uint32>::Max() || _Context.GetNewWorlds().IsEmpty())
    {
        return;
    }

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);

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

        const FString LocalAddress = WorldLocalAddress->ToString(false);
        if (!FIPv4Address::Parse(LocalAddress, mLocalAddress))
        {
            ensureMsgf(false, TEXT("We expect the local address to be valid"));
            continue;
        }

        if (_Context.GetIsArbitrerRunning())
        {
            mListenerSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocket"))
                              .AsNonBlocking()
                              .AsReusable()
                              .Build();

            mArbitrerTempMessageBuffer.Reserve(30);
            mArbitrerTempBuffer.Reserve(500);

            const FString ArbitrerLocalAdd = KL::Debug::Networking::Arbitrer::ArbitrerIPAddr.ToString();
            const UKLDebugNetworkingArbitrerSettings& ArbitrerSettings = UKLDebugNetworkingArbitrerSettings::Get();
            mArbitrerAddress = SocketSubsystem->CreateInternetAddr();
            bool IsValid = false;
            mArbitrerAddress->SetIp(*ArbitrerLocalAdd, IsValid);
            UE_CLOG(!IsValid, LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_InitLocalAddress>> Arbitrer could not send the IP"));
            if (IsValid)
            {
                *mArbitrerAddress = *WorldLocalAddress;
                mArbitrerAddress->SetPort(ArbitrerSettings.GetPort());
            }
        }

        break;
    }
}

void FKLDebugImGuiNetworkingTCPServer::GameThread_RemoveOldWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context)
{
    FMemoryWriter ArbitrerWriter{ mArbitrerTempBuffer };

    for (const FObjectKey& RemovedWorld : _Context.GetRemovedWorlds())
    {
        const int32 Index = mWorldCachedConnections.IndexOfByKey(RemovedWorld);
        if (Index == INDEX_NONE)
        {
            ensureMsgf(false, TEXT("should not happen"));
            continue;
        }

        RemoveCachedConnection(Index, ArbitrerWriter);
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
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> Could not get socket subsystem"));
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    FString LocalPort;
    int32 DebugPort = 0;

    FMemoryWriter ArbitrerWriter{ mArbitrerTempBuffer };

    uint32 StartWorldPort = Settings.Server_GetStartPortRange();
    uint32 EndWorldPort = Settings.Server_GetEndPortRange();
    const TOptional<KL::Debug::Server::Delegates::FServerSocketPortRangeDelegateData> OverridePortsData = KL::Debug::Server::Delegates::BroadcastOnGetDebugServerSocketPortRange();
    if (OverridePortsData.IsSet())
    {
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

        FSocket* WorldArbitrerSocket = GetNewWorldSocket(StartWorldPort, EndWorldPort, Settings, DebugPort);
        if (!WorldArbitrerSocket)
        {
            UE_LOG(LogKL_Debug, Display, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> Failed in creating arbitrer socket for world [%s]"), *AddeddWorld->GetName());
            continue;
        }

        const int32 WorldServerPort = WorldLocalAddress->GetPort();
        int32 ReceiveBufferSize = 0;
        int32 SenderBufferSize = 0;
        WorldArbitrerSocket->SetReceiveBufferSize(static_cast<int32>(Settings.Server_GetReadBufferSize()), ReceiveBufferSize);
        WorldArbitrerSocket->SetSendBufferSize(static_cast<int32>(Settings.Server_GetWriteBufferSize()), SenderBufferSize);
        mWorldCachedConnections.Emplace(*AddeddWorld.Get(), WorldServerPort, DebugPort, *WorldArbitrerSocket);

        if (_Context.GetIsArbitrerRunning())
        {
            ArbitrerNewWorldConnectionAdded(WorldServerPort, DebugPort, ArbitrerWriter);
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
        const FIPv4Endpoint Endpoint(mLocalAddress, static_cast<int32>(_DebugPort));
        NewSocket = FTcpSocketBuilder(TEXT("ClientListenerSocket"))
                    .AsNonBlocking()
                    .BoundToEndpoint(Endpoint)
                    .Listening(static_cast<int32>(_Settings.Server_GetMaxConnectionBacklog()))
                    .Build();
    }

    if (!NewSocket)
    {
        UE_LOG(LogKL_Debug, Display, TEXT("FKLDebugImGuiNetworkingTCPServer::GetNewWorldSocket>> Failed in creating socket"));
        return nullptr;
    }

    return NewSocket;
}

const TSharedPtr<FInternetAddr> FKLDebugImGuiNetworkingTCPServer::GetWorldLocalAddress(const UWorld& _World) const
{
    const UNetDriver* WorldNetDriver = _World.GetNetDriver();
    return WorldNetDriver ? WorldNetDriver->LocalAddr : nullptr;
}

void FKLDebugImGuiNetworkingTCPServer::ArbitrerNewWorldConnectionAdded(const int32 _ServerPort, const int32 _DebugPort, FArchive& _ArbitrerWriter)
{
    mArbitrerTempMessageBuffer.Reset();
    FMemoryWriter ArbitrerTempWriter{ mArbitrerTempMessageBuffer };

    FKLDebugNetworkingArbitrerMessage_ServerConnected ServerMessage{ static_cast<uint32>(_ServerPort), static_cast<uint32>(_DebugPort) };
    ServerMessage.Serialize(ArbitrerTempWriter);
    KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerMessage, mArbitrerTempMessageBuffer, _ArbitrerWriter);
}

void FKLDebugImGuiNetworkingTCPServer::ArbitrerRemovedWorldConnection(const int32 _ServerPort, FArchive& _ArbitrerWriter)
{
    mArbitrerTempMessageBuffer.Reset();
    FMemoryWriter ArbitrerTempWriter{ mArbitrerTempMessageBuffer };

    FKLDebugNetworkingArbitrerMessage_ServerDisconnected ServerMessage{ static_cast<uint32>(_ServerPort) };
    ServerMessage.Serialize(ArbitrerTempWriter);
    KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerMessage, mArbitrerTempMessageBuffer, _ArbitrerWriter);
}
