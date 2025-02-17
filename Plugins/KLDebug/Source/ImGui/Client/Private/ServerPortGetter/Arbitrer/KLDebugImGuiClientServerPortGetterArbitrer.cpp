// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ServerPortGetter/Arbitrer/KLDebugImGuiClientServerPortGetterArbitrer.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"
#include "Client/KLDebugImGuiClientWorldCacheConnection.h"

// modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Arbitrer/Public/Definitions/KLDebugNetworkingArbitrerDefinitions.h"
#include "Networking/Arbitrer/Public/Messages/Client/KLDebugNetworkingArbitrerMessage_ClientServerData.h"
#include "Networking/Arbitrer/Public/Settings/KLDebugNetworkingArbitrerSettings.h"
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Common/TcpSocketBuilder.h"
#include "Common/UdpSocketBuilder.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#if !NO_LOGGING
#include "Containers/UnrealString.h"
#endif

void FKLDebugImGuiClientServerPortGetterArbitrer::Init()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    const UKLDebugNetworkingArbitrerSettings& ArbitrerSettings = UKLDebugNetworkingArbitrerSettings::Get();
    mArbitrerAddress = SocketSubsystem->CreateInternetAddr();
    mArbitrerAddress->SetPort(ArbitrerSettings.GetPort());

    mTempAddress = SocketSubsystem->CreateInternetAddr();

    mArbitrerTempWriteBuffer.Reserve(500);
    mArbitrerWriteBuffer.Reserve(500);
    mArbitrerReplyReadData.Init(0, 500);
    mArbitrerConnections.Reserve(10);

    CreateArbitrerReplySocket(ArbitrerSettings);
}

void FKLDebugImGuiClientServerPortGetterArbitrer::CreateArbitrerReplySocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings)
{
    uint32 CurrentPort = _ArbitrerSettings.GetStartClientPortRange();
    while (!mArbitrerSocket && CurrentPort < _ArbitrerSettings.GetEndClientPortRange())
    {
        mArbitrerReplyPort = CurrentPort++;
        const FIPv4Endpoint Endpoint(FIPv4Address::Any, static_cast<int32>(mArbitrerReplyPort));
        mArbitrerSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocket"))
                              .AsNonBlocking()
                              .BoundToEndpoint(Endpoint)
                              .Build();
    }

    if (!mArbitrerSocket)
    {
        UE_LOG(LogKLDebug_Networking, Log, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::CreateArbitrerReplySocket>> we didnt manage to create the reply socket"));
        return;
    }

    int32 ReceiveBufferSize = 0;
    int32 SenderBufferSize = 0;
    mArbitrerSocket->SetReceiveBufferSize(static_cast<int32>(_ArbitrerSettings.GetReceiveBufferSize()), ReceiveBufferSize);
    mArbitrerSocket->SetSendBufferSize(static_cast<int32>(_ArbitrerSettings.GetWriteBufferSize()), SenderBufferSize);
}

void FKLDebugImGuiClientServerPortGetterArbitrer::Shutdown()
{
    if (!mArbitrerSocket)
    {
        return;
    }

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        return;
    }

    mArbitrerSocket->Close();
    SocketSubsystem->DestroySocket(mArbitrerSocket);
    mArbitrerSocket = nullptr;
}

void FKLDebugImGuiClientServerPortGetterArbitrer::AddNewConnections(const FKLDebugImGuiClientGameThreadContext& _Context)
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiClientArbitrerManager_AddNewConnections);

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    const UKLDebugNetworkingArbitrerSettings& ArbitrerSettings = UKLDebugNetworkingArbitrerSettings::Get();
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
            ensureMsgf(false, TEXT("Coult not createarbitrer address"));

            ArbitrerSocket->Close();
            SocketSubsystem->DestroySocket(ArbitrerSocket);
            continue;
        }

        ArbitrerAddress->SetIp(ServerIP);
        KL::Debug::Networking::Helpers::ChangeAddressToLocalIfLoopback(ArbitrerAddress.ToSharedRef());
        ArbitrerAddress->SetPort(ArbitrerSettings.GetPort());

#if !NO_LOGGING
        const FString ConnectionInfo = ArbitrerAddress->ToString(true);
        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::AddNewConnections>> Client connecting to arbitrer [%s]"), *ConnectionInfo);
#endif

        mArbitrerConnections.Emplace(*WorldPtr.Get(), LocalPlayerNetID, static_cast<uint32>(ServerPort), ArbitrerAddress.ToSharedRef(), *ArbitrerSocket);
    }
}

void FKLDebugImGuiClientServerPortGetterArbitrer::ParallelTick(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections)
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugImGuiClientArbitrerManager_Tick);

    checkf(mArbitrerSocket != nullptr, TEXT("arbitrer socket must be valid"));

    TickReadArbitrerData(_CachedConnections);
    TickPendingArbitrerConnections();
}

void FKLDebugImGuiClientServerPortGetterArbitrer::TickPendingArbitrerConnections()
{
    for (FKLDebugImGuiClientArbitrerCacheConnection& Connection : mArbitrerConnections)
    {
        mArbitrerTempWriteBuffer.Reset(),
            mArbitrerWriteBuffer.Reset();

        Connection.Parallel_Tick(mArbitrerReplyPort, mArbitrerTempWriteBuffer, mArbitrerWriteBuffer);
    }
}

void FKLDebugImGuiClientServerPortGetterArbitrer::TickReadArbitrerData(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections)
{
    uint32 Size = 0;
    int32 BytesRead = 0;

    while (mArbitrerSocket->HasPendingData(Size) && Size > 0)
    {
        mArbitrerReplyReadData.SetNumUninitialized(Size, EAllowShrinking::No);
        Size = 0;
        mArbitrerSocket->Recv(mArbitrerReplyReadData.GetData(), mArbitrerReplyReadData.Num(), BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::TickReadArbitrerData>> Socket had pending data but didnt read any bytes"));
            continue;
        }

        TArrayView<uint8> ReceiverView{ mArbitrerReplyReadData.GetData(), BytesRead };
        FMemoryReaderView Reader{ ReceiverView };
        OnReadArbitrerData(Reader, _CachedConnections);
    }
}

void FKLDebugImGuiClientServerPortGetterArbitrer::OnReadArbitrerData(FArchive& _Reader, TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections)
{
    if (_Reader.AtEnd())
    {
        return;
    }

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

    auto ReadMessagesLambda = [this, &Settings, &_CachedConnections](const FKLDebugNetworkingMessage_Header& _MessageHeader, FArchive& _MessageData) -> void {
        if (_MessageHeader.GetMessageEnumType() != static_cast<uint16>(EKLDebugNetworkingMessagesTypes::Arbitrer))
        {
            ensureMsgf(false, TEXT("received a message which was not expected"));
            return;
        }

        int32 Index = INDEX_NONE;
        switch (static_cast<EKLDebugArbitrerMessage>(_MessageHeader.GetMessageType()))
        {
        case EKLDebugArbitrerMessage::Client_GetServerData:
        {
            const FKLDebugNetworkingArbitrerMessage_ClientServerData ServerData{ _MessageData };
            Index = mArbitrerConnections.IndexOfByKey(ServerData.Client_GetClientID());
            if (Index == INDEX_NONE)
            {
                break;
            }

            if (!ServerData.Client_GetCanConnect())
            {
                UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::OnReadArbitrerData>> Client has header version different then the server. Cant connect to it removing"));
                break;
            }

            const FKLDebugImGuiClientArbitrerCacheConnection& CacheData = mArbitrerConnections[Index];
            if (CacheData.GetWorldObjKey().ResolveObjectPtr() == nullptr)
            {
                UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::OnReadArbitrerData>> Client is connecting to a non valid world anymore"));
                break;
            }

            OnArbitrerMessageRecv(CacheData, Settings, ServerData, _CachedConnections);
            break;
        }
        default:
            ensureMsgf(false, TEXT("not handled"));
            break;
        }

        if (Index != INDEX_NONE)
        {
            mArbitrerConnections.RemoveAtSwap(Index, 1, EAllowShrinking::No);
        }
    };

    KL::Debug::Networking::Message::ReadBufferGetStopReadLocation(ReadMessagesLambda, mArbitrerReplyTempData, _Reader);
}

void FKLDebugImGuiClientServerPortGetterArbitrer::OnArbitrerMessageRecv(const FKLDebugImGuiClientArbitrerCacheConnection& _ArbitrerConnection, const UKLDebugImGuiNetworkingSettings& _Settings, const FKLDebugNetworkingArbitrerMessage_ClientServerData& _Data, TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections)
{
    checkf(mTempAddress.IsValid(), TEXT("mTempAddress must be valid"));

    mTempAddress->SetIp(_ArbitrerConnection.GetHost());
    mTempAddress->SetPort(_Data.Client_GetDebugPort());
    FSocket* NewSocket = FTcpSocketBuilder(TEXT("ClientDebugSocket"))
                             .AsNonBlocking()
                             .Build();

    if (!NewSocket)
    {
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::OnArbitrerMessageRecv>> Could not create socket"));
        return;
    }

    int32 ReceiveBufferSize = 0;
    int32 SenderBufferSize = 0;
    NewSocket->SetReceiveBufferSize(static_cast<int32>(_Settings.Server_GetReadBufferSize()), ReceiveBufferSize);
    NewSocket->SetSendBufferSize(static_cast<int32>(_Settings.Server_GetWriteBufferSize()), SenderBufferSize);

#if !NO_LOGGING
    const FString ConnectionInfo = mTempAddress->ToString(true);
    UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::TickPendingWorlds>> Client connecting to debug port in remote server [%s]"), *ConnectionInfo);
#endif

    if (!NewSocket->Connect(*mTempAddress))
    {
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiClientServerPortGetterArbitrer::OnArbitrerMessageRecv>> Could not connect to server"));
        return;
    }

    _CachedConnections.Emplace(_ArbitrerConnection.GetWorldObjKey(), _ArbitrerConnection.GetNetLocalPlayerID(), ReceiveBufferSize, SenderBufferSize, *NewSocket);
}
