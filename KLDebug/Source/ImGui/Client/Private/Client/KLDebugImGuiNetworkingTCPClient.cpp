// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiNetworkingTCPClient.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"
#include "Subsystem/Engine/KLDebugImGuiClientSubsystem_Engine.h"

// modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Arbitrer/Public/Messages/Client/KLDebugNetworkingArbitrerMessage_ClientServerData.h"
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
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"
#include "Misc/ScopeLock.h"
#include "Misc/ScopeTryLock.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "SocketSubsystem.h"

void FKLDebugImGuiNetworkingTCPClient::CreateSocket()
{
    mListenerSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocket"))
                      .AsNonBlocking()
                      .AsReusable()
                      .Build();

    mCachedConnections.Reserve(3);

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    const UKLDebugNetworkingArbitrerSettings& ArbitrerSettings = UKLDebugNetworkingArbitrerSettings::Get();
    mArbitrerAddress = SocketSubsystem->CreateInternetAddr();
    mArbitrerAddress->SetPort(ArbitrerSettings.GetPort());

    mTempAddress = SocketSubsystem->CreateInternetAddr();

    mArbitrerTempWriteBuffer.Reserve(500);
    mArbitrerWriteBuffer.Reserve(500);

    CreateArbitrerReplySocket(ArbitrerSettings);
}

void FKLDebugImGuiNetworkingTCPClient::CreateArbitrerReplySocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings)
{
    const FIPv4Address IPAddr(127, 0, 0, 1);
    uint32 CurrentPort = _ArbitrerSettings.GetStartClientPortRange();
    while (!mArbitrerReplySocket && CurrentPort < _ArbitrerSettings.GetEndClientPortRange())
    {
        mArbitrerReplyPort = CurrentPort++;
        const FIPv4Endpoint Endpoint(IPAddr, static_cast<int32>(mArbitrerReplyPort));
        mArbitrerReplySocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocket"))
                               .AsNonBlocking()
                               .BoundToEndpoint(Endpoint)
                               .Build();
    }

    if (!mArbitrerReplySocket)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("FKLDebugImGuiNetworkingTCPClient::CreateArbitrerReplySocket>> we didnt manage to create the reply socket"));
        return;
    }

    int32 ReceiveBufferSize = 0;
    int32 SenderBufferSize = 0;
    mArbitrerReplySocket->SetReceiveBufferSize(static_cast<int32>(_ArbitrerSettings.GetReceiveBufferSize()), ReceiveBufferSize);
    mArbitrerReplySocket->SetSendBufferSize(static_cast<int32>(_ArbitrerSettings.GetWriteBufferSize()), SenderBufferSize);
}

void FKLDebugImGuiNetworkingTCPClient::Exit()
{
    FKLDebugImGuiNetworkingTCPBase::Exit();

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (mArbitrerReplySocket && SocketSubsystem)
    {
        mArbitrerReplySocket->Close();
        SocketSubsystem->DestroySocket(mArbitrerReplySocket);
        mArbitrerReplySocket = nullptr;
    }
}

void FKLDebugImGuiNetworkingTCPClient::RemoveCachedConnection(const int32 _Index)
{
    FKLDebugImGuiClientWorldCacheConnection& CachedConnection = mCachedConnections[_Index];
    mCachedConnections.RemoveAtSwap(_Index, 1, false);
}

void FKLDebugImGuiNetworkingTCPClient::RunChild()
{
    TickReadArbitrerData();
    TickConnections();

#if !WITH_EDITOR
    // in package builds, once we disconnect from the server to connect to a new one, recreate the socket
    // so we are listening again for new connections
    if (mCachedConnections.IsEmpty())
    {
        InitSocket();
    }
#endif
}

void FKLDebugImGuiNetworkingTCPClient::TickConnections()
{
    bool RequiresGameThreadTick = false;

    for (int32 i = mCachedConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiClientWorldCacheConnection& CachedConnection = mCachedConnections[i];
        RequiresGameThreadTick |= CachedConnection.Parallel_Tick(mArbitrerReplyPort, mArbitrerTempWriteBuffer, mArbitrerWriteBuffer);
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

void FKLDebugImGuiNetworkingTCPClient::TickReadArbitrerData()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiNetworkingTCPClient_TickReadArbitrerData);

    uint32 Size = 0;
    int32 BytesRead = 0;

    while (mArbitrerReplySocket->HasPendingData(Size) && Size > 0)
    {
        mArbitrerReplyReadData.Reset();
        mArbitrerReplyReadData.AddUninitialized(Size);
        Size = 0;
        mArbitrerReplySocket->Recv(mArbitrerReplyReadData.GetData(), mArbitrerReplyReadData.Num(), BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiNetworkingTCPClient::TickReadArbitrerData>> Socket had pending data but didnt read any bytes"));
            continue;
        }

        FMemoryReader Reader{ mArbitrerReplyReadData };
        OnReadArbitrerData(Reader);
    }
}

void FKLDebugImGuiNetworkingTCPClient::OnReadArbitrerData(FArchive& _Reader)
{
    if (_Reader.AtEnd())
    {
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    const int64 TotalSize = _Reader.TotalSize();
    while (!_Reader.AtEnd())
    {
        const int64 HeaderCurrentPosition = _Reader.Tell();
        if (TotalSize - HeaderCurrentPosition < KL::Debug::Networking::Message::GetHeaderSize())
        {
            // not enough data for the header
            break;
        }

        const FKLDebugNetworkingMessage_Header HeaderMessage{ _Reader };
        if (!HeaderMessage.IsValid())
        {
            // garbage in the stream skip one byte
            _Reader.Seek(HeaderCurrentPosition + 1);
            continue;
        }

        const int64 CurrentReadBufferPosition = _Reader.Tell();
        const int64 RemainingSpace = TotalSize - CurrentReadBufferPosition;
        if (RemainingSpace < HeaderMessage.GetMessageDataSize())
        {
            ensureMsgf(false, TEXT("we dont expect this here"));
            break;
        }

        mArbitrerReplyTempData.SetNum(static_cast<int32>(HeaderMessage.GetMessageDataSize()), false);
        _Reader.Serialize(mArbitrerReplyTempData.GetData(), HeaderMessage.GetMessageDataSize());
        FMemoryReader MessageMemory(mArbitrerReplyTempData);

        const FKLDebugNetworkingArbitrerMessage_ClientServerData ServerData{ MessageMemory };
        const int32 Index = mCachedConnections.IndexOfByKey(ServerData.Client_GetClientID());
        if (Index == INDEX_NONE)
        {
            continue;
        }

        if (!ServerData.Client_GetCanConnect())
        {
            UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiNetworkingTCPClient::OnReadArbitrerData>> Client has header version different then the server. Cant connect to it removing"));
            mCachedConnections.RemoveAtSwap(Index, 1, false);
            continue;
        }

        FKLDebugImGuiClientWorldCacheConnection& CacheData = mCachedConnections[Index];
        CacheData.OnArbitrerMessageRecv(Settings, ServerData, *mTempAddress.Get());
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

    GameThread_NewWorlds(_Context);
    GameThread_RemoveInvalidWorlds(_Context);
    GameThread_TickImGuiData(_Context);
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

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    const UKLDebugNetworkingArbitrerSettings& ArbitrerSettings = UKLDebugNetworkingArbitrerSettings::Get();

    for (const TWeakObjectPtr<const UWorld>& WorldPtr : _Context.GetNewWorlds())
    {
        if (!WorldPtr.IsValid())
        {
            continue;
        }

        const UNetDriver* WorldNetDriver = WorldPtr->GetNetDriver();
        const UNetConnection* NetConnection = WorldNetDriver ? WorldNetDriver->ServerConnection : nullptr;
        if (!NetConnection)
        {
            ensureMsgf(false, TEXT("we expect the world to be fully initialize here"));
            continue;
        }

        FSocket* ArbitrerSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ClientSocket"))
                                  .AsNonBlocking()
                                  .AsReusable()
                                  .Build();

        if (!ArbitrerSocket)
        {
            ensureMsgf(false, TEXT("Coult not create an arbitrer socket"));
            continue;
        }

        TSharedPtr<FInternetAddr> ArbitrerAddress = SocketSubsystem->CreateInternetAddr();
        if (!ArbitrerAddress.IsValid())
        {
            ArbitrerSocket->Close();
            SocketSubsystem->DestroySocket(ArbitrerSocket);

            ensureMsgf(false, TEXT("Coult not create an arbitrer address"));
            continue;
        }

        bool IsValid = false;
        ArbitrerAddress->SetIp(*NetConnection->URL.Host, IsValid);
        if (!IsValid)
        {
            ensureMsgf(false, TEXT("Coult not store IP address to world"));
            continue;
        }

        ArbitrerAddress->SetPort(ArbitrerSettings.GetPort());
        mCachedConnections.Emplace(*WorldPtr.Get(), NetConnection->URL.Host, static_cast<uint32>(NetConnection->URL.Port), ArbitrerAddress.ToSharedRef(), *ArbitrerSocket);
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
