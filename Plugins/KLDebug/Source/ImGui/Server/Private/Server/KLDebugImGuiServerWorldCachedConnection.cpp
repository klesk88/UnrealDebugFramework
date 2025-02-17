// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiServerWorldCachedConnection.h"

// modules
#include "ImGui/Networking/Public/Commands/Manager/KLDebugNetworkingCommandManager.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "ImGui/Networking/Public/Commands/Message/KLDebugNetworkingMessage_Command.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessageHeaderDefinitions.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "User/Framework/Internal/Networking/Commands/Helpers/KLDebugUserNetworkingCommandsRequester.h"

// engine
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiServerWorldCachedConnection::FKLDebugImGuiServerWorldCachedConnection(const UWorld& _World, const int32 _ServerPort, const int32 _DebugPort, FSocket& _WorldDebugSocket)
    : mWorldKey(&_World)
    , mServerPort(_ServerPort)
    , mDebugPort(_DebugPort)
    , mWorldDebugSocket(&_WorldDebugSocket)
{
    KL::Debug::Networking::Commands::Manager::BindOnNewCommandWorldRequest(mWorldKey, FOnKLDebugNewCommandWorld::CreateRaw(this, &FKLDebugImGuiServerWorldCachedConnection::OnNewCommandRequest));
}

FKLDebugImGuiServerWorldCachedConnection::~FKLDebugImGuiServerWorldCachedConnection()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    ShutdownInternal(*SocketSubsystem);

    KL::Debug::Networking::Commands::Manager::UnbindOnNewCommandWorldRequest(mWorldKey);
}

void FKLDebugImGuiServerWorldCachedConnection::ShutdownInternal(ISocketSubsystem& _SocketSubsystem)
{
    if (mWorldDebugSocket)
    {
        mWorldDebugSocket->Close();
        _SocketSubsystem.DestroySocket(mWorldDebugSocket);
        mWorldDebugSocket = nullptr;
    }
}

void FKLDebugImGuiServerWorldCachedConnection::RemoveInvalidClientConnections()
{
    for (int32 i = mCachedClientConnections.Num() - 1; i >= 0; --i)
    {
        const FKLDebugImGuiServerClientCachedConnection& ClientConnection = mCachedClientConnections[i];
        if (!ClientConnection.IsValid())
        {
            mCachedClientConnections.RemoveAtSwap(i, 1, EAllowShrinking::No);
        }
    }
}

bool FKLDebugImGuiServerWorldCachedConnection::Parallel_Tick()
{
    TickPendingConnections();
    return TickClientsConnections();
}

void FKLDebugImGuiServerWorldCachedConnection::TickPendingConnections()
{
    checkf(mWorldDebugSocket != nullptr, TEXT("must be valid"));

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

    bool bHasPendingConnection = false;
    while (mWorldDebugSocket->HasPendingConnection(bHasPendingConnection) && bHasPendingConnection)
    {
        FSocket* IncomingConnection = mWorldDebugSocket->Accept(TEXT("ClientRequest"));
        if (!IncomingConnection)
        {
            UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiServerWorldCachedConnection::TickPendingConnections>> Could not accept socket"));
            continue;
        }

        int32 RecvSize = 0;
        int32 WriteSize = 0;
        IncomingConnection->SetReceiveBufferSize(static_cast<int32>(Settings.Server_GetReadBufferSize()), RecvSize);
        IncomingConnection->SetSendBufferSize(static_cast<int32>(Settings.Server_GetWriteBufferSize()), WriteSize);

        mCachedClientConnections.Emplace(RecvSize, WriteSize, *IncomingConnection);
    }
}

bool FKLDebugImGuiServerWorldCachedConnection::TickClientsConnections()
{
    bool HasNewReadData = false;
    for (int32 i = mCachedClientConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiServerClientCachedConnection& Connection = mCachedClientConnections[i];
        if (!Connection.IsValid())
        {
            UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiServerWorldCachedConnection::TickClientsConnections>> Removing client connection because marked invalid"));
            mCachedClientConnections.RemoveAtSwap(i, 1, EAllowShrinking::No);
            continue;
        }

        Connection.Tick();
        HasNewReadData |= Connection.RequiresGameThreadTick();
    }

    return HasNewReadData;
}

bool FKLDebugImGuiServerWorldCachedConnection::TickOnGameThread()
{
    RemoveInvalidConnections();
    if (mCachedClientConnections.IsEmpty())
    {
        return false;
    }

    UWorld* World = Cast<UWorld>(mWorldKey.ResolveObjectPtr());
    checkf(World != nullptr, TEXT("must be valid"));

    TArray<FKLDebugNetworkingCommandConnectionManagerBase*> ClientsCommandMangers;
    ClientsCommandMangers.Reserve(mCachedClientConnections.Num());

    for (FKLDebugImGuiServerClientCachedConnection& Client : mCachedClientConnections)
    {
        ClientsCommandMangers.Emplace(&Client.GetCommandManagerMutable());
    }

    bool KeepTicking = false;
    for (int32 i = mCachedClientConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiServerClientCachedConnection& Connection = mCachedClientConnections[i];
        if (!Connection.IsValid())
        {
            UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiServerWorldCachedConnection::TickOnGameThread>> Removing client connection because marked invalid"));
            mCachedClientConnections.RemoveAtSwap(i, 1, EAllowShrinking::No);
            continue;
        }

        KeepTicking |= Connection.TickOnGameThread(ClientsCommandMangers, *World);
    }

    return KeepTicking;
}

void FKLDebugImGuiServerWorldCachedConnection::RemoveInvalidConnections()
{
    for (int32 i = mCachedClientConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiServerClientCachedConnection& Connection = mCachedClientConnections[i];
        if (!Connection.IsValid())
        {
            UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiServerWorldCachedConnection::TickOnGameThread>> Removing client connection because marked invalid"));
            mCachedClientConnections.RemoveAtSwap(i, 1, EAllowShrinking::No);
            continue;
        }
    }
}

void FKLDebugImGuiServerWorldCachedConnection::OnNewCommandRequest(const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& _Input)
{
    if (_Input.GetClientFilter())
    {
        for (FKLDebugImGuiServerClientCachedConnection& Client : mCachedClientConnections)
        {
            if (_Input.GetContextData().IsEmpty())
            {
                Client.AddCommand(FKLDebugNetworkingMessage_Command(_Input.GetCommandID()));
            }
            else
            {
                Client.AddCommand(FKLDebugNetworkingMessage_Command(_Input.GetCommandID(), _Input.GetContextDataMutable()));
            }
        }
    }
    else
    {
        for (FKLDebugImGuiServerClientCachedConnection& Client : mCachedClientConnections)
        {
            const APlayerController* ClientOwningPlayer = Client.TryGetPlayerController();
            if (!ClientOwningPlayer)
            {
                UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugImGuiServerWorldCachedConnection::OnNewCommandRequest>> Client connection didnt have a valid player character. Will not send to client"));
                continue;
            }

            if (!_Input.GetClientFilter()(*ClientOwningPlayer))
            {
                continue;
            }

            if (_Input.GetContextData().IsEmpty())
            {
                Client.AddCommand(FKLDebugNetworkingMessage_Command(_Input.GetCommandID()));
            }
            else
            {
                Client.AddCommand(FKLDebugNetworkingMessage_Command(_Input.GetCommandID(), _Input.GetContextDataMutable()));
            }
        }
    }
}
