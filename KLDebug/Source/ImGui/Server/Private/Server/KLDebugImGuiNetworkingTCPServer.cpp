// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiNetworkingTCPServer.h"

#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"
#include "Subsystem/Engine/KLDebugImGuiServerSubsystem_Engine.h"

// modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
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

void FKLDebugImGuiNetworkingTCPServer::CreateSocket()
{
    mListenerSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocket"))
                      .AsNonBlocking()
                      .AsReusable()
                      .Build();

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    const UKLDebugNetworkingArbitrerSettings& ArbitrerSettings = UKLDebugNetworkingArbitrerSettings::Get();
    mArbitrerAddress = SocketSubsystem->CreateInternetAddr();
    bool IsValid = false;
    mArbitrerAddress->SetIp(TEXT("127.0.0.1"), IsValid);
    checkf(IsValid, TEXT("must be valid"));
    mArbitrerAddress->SetPort(ArbitrerSettings.GetPort());

    mArbitrerTempMessageBuffer.Reserve(500);
    mArbitrerTempBuffer.Reserve(500);
}

void FKLDebugImGuiNetworkingTCPServer::RemoveCachedConnection(const int32 _Index)
{
    FKLDebugImGuiServerWorldCachedConnection& RemoveConnection = mWorldCachedConnections[_Index];
    RemoveConnection.OnRemove(*mArbitrerAddress.Get(), *mListenerSocket, mArbitrerTempMessageBuffer, mArbitrerTempBuffer);
    mWorldCachedConnections.RemoveAtSwap(_Index, 1, false);
}

void FKLDebugImGuiNetworkingTCPServer::RunChild()
{
    Parallel_TickCachedConnections();
}

void FKLDebugImGuiNetworkingTCPServer::Parallel_TickCachedConnections()
{
    checkf(mArbitrerAddress.IsValid(), TEXT("Internet address must be valid"));

    bool RequiresGameThreadTick = false;
    for (FKLDebugImGuiServerWorldCachedConnection& WorldConnection : mWorldCachedConnections)
    {
        RequiresGameThreadTick |= WorldConnection.Parallel_Tick(*mArbitrerAddress.Get(), *mListenerSocket, mArbitrerTempMessageBuffer, mArbitrerTempBuffer);
    }

    // this works without locks because we are owned by the KLDebugImGuiServerSubsystem_Engine
    // so we are sure the object is valid
    if (RequiresGameThreadTick)
    {
        UKLDebugImGuiServerSubsystem_Engine* EngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
        EngineSubsystem->SetShouldTick();
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
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> Could not get socket subsystem"));
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    const FString LocalHost(TEXT("127.0.0.1"));
    int32 DebugPort = 0;

    for (const TWeakObjectPtr<const UWorld>& AddeddWorld : _Context.GetNewWorlds())
    {
        if (!AddeddWorld.IsValid())
        {
            continue;
        }

        const UNetDriver* WorldNetDriver = AddeddWorld->GetNetDriver();
        const TSharedPtr<FInternetAddr> WorldLocalAddress = WorldNetDriver ? WorldNetDriver->LocalAddr : nullptr;
        if (!WorldLocalAddress.IsValid())
        {
            ensureMsgf(false, TEXT("No valid net connection for world. Not expected"));
            continue;
        }

        FSocket* WorldArbitrerSocket = GetNewWorldSocket(Settings, DebugPort);
        if (!WorldArbitrerSocket)
        {
            UE_LOG(LogKL_Debug, Log, TEXT("FKLDebugImGuiNetworkingTCPServer::GameThread_AddNewWorlds>> Failed in creating arbitrer socket for world [%s]"), *AddeddWorld->GetName());
            continue;
        }

        int32 ReceiveBufferSize = 0;
        int32 SenderBufferSize = 0;
        WorldArbitrerSocket->SetReceiveBufferSize(static_cast<int32>(Settings.Server_GetReadBufferSize()), ReceiveBufferSize);
        WorldArbitrerSocket->SetSendBufferSize(static_cast<int32>(Settings.Server_GetWriteBufferSize()), SenderBufferSize);
        mWorldCachedConnections.Emplace(*AddeddWorld.Get(), WorldLocalAddress->GetPort(), DebugPort, *WorldArbitrerSocket);
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

FSocket* FKLDebugImGuiNetworkingTCPServer::GetNewWorldSocket(const UKLDebugImGuiNetworkingSettings& _Settings, int32& _DebugPort) const
{
    const FIPv4Address IPAddr(127, 0, 0, 1);
    uint32 CurrentPort = _Settings.Server_GetStartPortRange();
    FSocket* NewSocket = nullptr;
    while (!NewSocket && CurrentPort < static_cast<int32>(_Settings.Server_GetEndPortRange()))
    {
        _DebugPort = CurrentPort++;
        const FIPv4Endpoint Endpoint(IPAddr, static_cast<int32>(_DebugPort));
        NewSocket = FTcpSocketBuilder(TEXT("ClientListenerSocket"))
                    .AsNonBlocking()
                    .BoundToEndpoint(Endpoint)
                    .Listening(static_cast<int32>(_Settings.Server_GetMaxConnectionBacklog()))
                    .Build();
    }

    if (!NewSocket)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("FKLDebugImGuiNetworkingTCPServer::GetNewWorldSocket>> Failed in creating socket"));
        return nullptr;
    }

    return NewSocket;
}
