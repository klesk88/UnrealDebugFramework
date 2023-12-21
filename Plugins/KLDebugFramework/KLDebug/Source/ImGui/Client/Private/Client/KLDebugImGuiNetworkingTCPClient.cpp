// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiNetworkingTCPClient.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"
#include "Delegates/KLDebugImGuiClientDelegates.h"
#include "ServerPortGetter/Arbitrer/KLDebugImGuiClientServerPortGetterArbitrer.h"
#include "ServerPortGetter/User/KLDebugImGuiClientServerPortGetterUser.h"
#include "Subsystem/Engine/KLDebugImGuiClientSubsystem_Engine.h"

// modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"

// engine
#include "Common/TcpSocketBuilder.h"
#include "Common/UdpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "Engine/NetDriver.h"
#include "Engine/World.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"
#include "Misc/ScopeLock.h"
#include "Misc/ScopeTryLock.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "SocketSubsystem.h"
#include "Templates/SharedPointer.h"

void FKLDebugImGuiNetworkingTCPClient::CreateSocket()
{
    mCachedConnections.Reserve(3);
}

void FKLDebugImGuiNetworkingTCPClient::Exit()
{
    FKLDebugImGuiNetworkingTCPBase::Exit();

    if (mServerPortGetter.IsValid())
    {
        mServerPortGetter->Shutdown();
    }
}

void FKLDebugImGuiNetworkingTCPClient::RemoveCachedConnection(const int32 _Index)
{
    FKLDebugImGuiClientWorldCacheConnection& CachedConnection = mCachedConnections[_Index];
    mCachedConnections.RemoveAtSwap(_Index, 1, false);
}

void FKLDebugImGuiNetworkingTCPClient::RunChild()
{
    TickPortGetter();
    TickConnections();
}

void FKLDebugImGuiNetworkingTCPClient::TickPortGetter()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiNetworkingTCPClient_TickPortGetter);

    if (mServerPortGetter.IsValid())
    {
        mServerPortGetter->ParallelTick(mCachedConnections);
    }
}

void FKLDebugImGuiNetworkingTCPClient::TickConnections()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiNetworkingTCPClient_TickConnections);

    bool RequiresGameThreadTick = false;

    for (int32 i = mCachedConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiClientWorldCacheConnection& CachedConnection = mCachedConnections[i];
        RequiresGameThreadTick |= CachedConnection.Parallel_Tick();
    }

    // this works without locks because we are sure the subsystem exists because it is owning us
    if (RequiresGameThreadTick)
    {
        UKLDebugImGuiClientSubsystem_Engine* EngineSubsystem = UKLDebugImGuiClientSubsystem_Engine::TryGetMutable();
        if (RequiresGameThreadTick)
        {
            EngineSubsystem->SetShouldTick();
        }
    }
}

void FKLDebugImGuiNetworkingTCPClient::TickGameThread(FKLDebugImGuiClientGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPClient_TickGameThread);

    // perform the update from the game thread. Here we are sure we can access actors and other systems without possibly causing
    // crashes due to lock or other things

    FScopeTryLock TryLock(&mGameThreadUpdateLock);
    if (!TryLock.IsLocked())
    {
        _Context.SetShouldKeepTicking(true);
        return;
    }

    GameThread_InitServerPortGetter(_Context);
    GameThread_NewWorlds(_Context);
    GameThread_RemoveInvalidWorlds(_Context);
    GameThread_TickImGuiData(_Context);
}

void FKLDebugImGuiNetworkingTCPClient::GameThread_InitServerPortGetter(const FKLDebugImGuiClientGameThreadContext& _Context)
{
    if (mServerPortGetter.IsValid())
    {
        return;
    }

    if (KL::Debug::Client::Delegates::IsGetDebugServerSocketPortBound())
    {
        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiNetworkingTCPClient::GameThread_InitServerPortGetter>> using user server port getter"));
        mServerPortGetter = MakeUnique<FKLDebugImGuiClientServerPortGetterUser>();
    }
    else
    {
        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiNetworkingTCPClient::GameThread_InitServerPortGetter>> using arbitrer server port getter"));
        mServerPortGetter = MakeUnique<FKLDebugImGuiClientServerPortGetterArbitrer>();
    }

    mServerPortGetter->Init();
}

void FKLDebugImGuiNetworkingTCPClient::GameThread_RemoveInvalidWorlds(const FKLDebugImGuiClientGameThreadContext& _Context)
{
    for (const FObjectKey& RemovedWorld : _Context.GetRemovedWorlds())
    {
        const int32 Index = mCachedConnections.IndexOfByKey(RemovedWorld);
        if (Index != INDEX_NONE)
        {
            RemoveCachedConnection(Index);
        }
    }
}

void FKLDebugImGuiNetworkingTCPClient::GameThread_NewWorlds(const FKLDebugImGuiClientGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPClient_GameThread_NewWorlds);

    if (mServerPortGetter.IsValid())
    {
        mServerPortGetter->AddNewConnections(_Context);
    }
}

void FKLDebugImGuiNetworkingTCPClient::GameThread_TickImGuiData(FKLDebugImGuiClientGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPClient_TickImGuiData);

    bool KeepTicking = false;
    for (FKLDebugImGuiClientData& ClientData : _Context.GetClientDataMutable())
    {
        FKLDebugImGuiClientWorldCacheConnection* CachedConnection = mCachedConnections.FindByKey(ClientData.GetWorldID());
        if (!CachedConnection)
        {
            continue;
        }

        KeepTicking |= CachedConnection->TickOnGameThread(ClientData);
    }

    _Context.SetShouldKeepTicking(KeepTicking);
}
