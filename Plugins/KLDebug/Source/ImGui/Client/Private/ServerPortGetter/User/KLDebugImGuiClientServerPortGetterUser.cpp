// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ServerPortGetter/User/KLDebugImGuiClientServerPortGetterUser.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"
#include "Client/KLDebugImGuiClientWorldCacheConnection.h"
#include "Networking/Client/KLDebugNetworkingClientDelegates.h"

// modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"

// engine
#include "Common/TcpSocketBuilder.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "Engine/NetDriver.h"
#include "Engine/World.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#if !NO_LOGGING
#include "Containers/UnrealString.h"
#endif

void FKLDebugImGuiClientServerPortGetterUser::Init()
{
    mPendingWorlds.Reserve(10);

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    mTempAddress = SocketSubsystem->CreateInternetAddr();
}

void FKLDebugImGuiClientServerPortGetterUser::AddNewConnections(const FKLDebugImGuiClientGameThreadContext& _Context)
{
    uint32 ServerIP = 0;
    int32 ServerPort = 0;
    FUniqueNetIdRepl LocalPlayerNetID;

    for (const TWeakObjectPtr<const UWorld>& WorldPtr : _Context.GetNewWorlds())
    {
        if (!WorldPtr.IsValid())
        {
            continue;
        }

        if (!GatherWorldData(*WorldPtr.Get(), ServerIP, ServerPort, LocalPlayerNetID))
        {
            continue;
        }

        mPendingWorlds.Emplace(*WorldPtr.Get(), ServerIP, LocalPlayerNetID);
    }
}

void FKLDebugImGuiClientServerPortGetterUser::RemoveWorlds(const TArray<FObjectKey>& _RemovedWorlds)
{
    for (const FObjectKey& RemovedWorld : _RemovedWorlds)
    {
        const int32 Index = mPendingWorlds.IndexOfByKey(RemovedWorld);
        if (Index != INDEX_NONE)
        {
            mPendingWorlds.RemoveAtSwap(Index, 1, false);
        }
    }
}

void FKLDebugImGuiClientServerPortGetterUser::ParallelTick(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections)
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiClientServerPortGetterUser_Tick);

    if (mPendingWorlds.IsEmpty())
    {
        return;
    }

    const TOptional<uint32> Result = KL::Debug::Client::Delegates::Internal::BroadcastGetDebugServerSocketPort();
    if (!Result.IsSet())
    {
        // the user can do its own logic to pass us the port which can take time (for example communicating with a remote
        // virtual machine). So it can happen we still dont have valid data here
        return;
    }

    TickPendingWorlds(Result.GetValue(), _CachedConnections);
    mPendingWorlds.Reset();
}

void FKLDebugImGuiClientServerPortGetterUser::TickPendingWorlds(const uint32 _ServerPort, TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections)
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

#if !NO_LOGGING
    FString ConnectionInfo;
#endif

    for (const FKLDebugImGuiClientServerPortGetterUser_PendingWorld& PendingWorld : mPendingWorlds)
    {
        mTempAddress->SetIp(PendingWorld.GetServerIP());
        FSocket* NewSocket = FTcpSocketBuilder(TEXT("ClientDebugSocket"))
                                 .AsNonBlocking()
                                 .Build();

        if (!NewSocket)
        {
            UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiClientServerPortGetterUser::TickPendingWorlds>> Could not create socket"));
            return;
        }

        mTempAddress->SetPort(static_cast<int32>(_ServerPort));
        int32 ReceiveBufferSize = 0;
        int32 SenderBufferSize = 0;
        NewSocket->SetReceiveBufferSize(static_cast<int32>(Settings.Server_GetReadBufferSize()), ReceiveBufferSize);
        NewSocket->SetSendBufferSize(static_cast<int32>(Settings.Server_GetWriteBufferSize()), SenderBufferSize);

#if !NO_LOGGING
        ConnectionInfo = mTempAddress->ToString(true);
        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiClientServerPortGetterUser::TickPendingWorlds>> Client connecting to debug port in remote server [%s]"), *ConnectionInfo);
#endif

        if (!NewSocket->Connect(*mTempAddress))
        {
            UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiClientServerPortGetterUser::TickPendingWorlds>> Could not connect to server"));
            return;
        }

        _CachedConnections.Emplace(PendingWorld.GetWorldKey(), PendingWorld.GetNetLocalPlayerID(), ReceiveBufferSize, SenderBufferSize, *NewSocket);
    }
}
