// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugArbitrer_Server.h"

// modules
#include "Networking/Arbitrer/Public/Definitions/KLDebugNetworkingArbitrerDefinitions.h"
#include "Networking/Arbitrer/Public/Log/KLDebugNetworkingArbitrerLog.h"
#include "Networking/Arbitrer/Public/Messages/Client/KLDebugNetworkingArbitrerMessage_ClientServerData.h"
#include "Networking/Arbitrer/Public/Messages/KLDebugNetworkingArbitrerMessage_Types.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerConnected.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerDisconnected.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerPing.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerPong.h"
#include "Networking/Arbitrer/Public/Settings/KLDebugNetworkingArbitrerSettings.h"
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageDefinitions.h"

// engine
#include "Common/UdpSocketBuilder.h"
#include "Containers/ArrayView.h"
#include "Containers/UnrealString.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Templates/SharedPointer.h"

bool FKLDebugNetworkArbitrer_Server::Init()
{
    CreateSocket();
    CreateClientSocket();

    const UKLDebugNetworkingArbitrerSettings& Settings = UKLDebugNetworkingArbitrerSettings::Get();
    mReceiveBuffer.Init(0, Settings.GetReceiveBufferSize());
    mTempMessageBuffer.Reserve(Settings.GetWriteBufferSize());
    mPendingMessages.Reserve(10);
    mTempSendMessageBuffer.Reserve(500);
    mSendMessageBuffer.Reserve(500);

    if (ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM))
    {
        mTempClientAddress = SocketSubsystem->CreateInternetAddr(TEXT("ClientTempAddress"));
    }
    else
    {
        ensureMsgf(false, TEXT("could not find socket subsystem not expected"));
    }

    return mListenerSocket != nullptr;
}

void FKLDebugNetworkArbitrer_Server::Exit()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    checkf(SocketSubsystem != nullptr, TEXT("must be valid"));
    ClearListenerSocket(*SocketSubsystem);
    ClearClientSocket(*SocketSubsystem);
}

void FKLDebugNetworkArbitrer_Server::ClearListenerSocket(ISocketSubsystem& _SocketSubsystem)
{
    if (mListenerSocket)
    {
        mListenerSocket->Close();
        _SocketSubsystem.DestroySocket(mListenerSocket);
        mListenerSocket = nullptr;
    }
}

void FKLDebugNetworkArbitrer_Server::ClearClientSocket(ISocketSubsystem& _SocketSubsystem)
{
    if (mClientSocket)
    {
        mClientSocket->Close();
        _SocketSubsystem.DestroySocket(mListenerSocket);
        mClientSocket = nullptr;
    }
}

void FKLDebugNetworkArbitrer_Server::CreateSocket()
{
    InitListenerSocket();
}

void FKLDebugNetworkArbitrer_Server::CreateClientSocket()
{
    mClientSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ClientSocket"))
                    .AsNonBlocking()
                    .Build();
}

void FKLDebugNetworkArbitrer_Server::InitListenerSocket()
{
    const UKLDebugNetworkingArbitrerSettings& Settings = UKLDebugNetworkingArbitrerSettings::Get();
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem != nullptr);

    const FIPv4Endpoint Endpoint(FIPv4Address::Any, Settings.GetPort());
    mListenerSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocket"))
                      .AsNonBlocking()
                      .BoundToEndpoint(Endpoint)
                      .Build();

    const FString ConnectionInfo = Endpoint.ToString();
    UE_LOG(LogKL_DebugArbitrer, Display, TEXT("FKLDebugNetworkArbitrer_Server::InitListenerSocket>> Arbitrer listening on [%s]"), *ConnectionInfo);
}

void FKLDebugNetworkArbitrer_Server::Run()
{
    if (!mListenerSocket)
    {
        return;
    }

    TickReadData();
    TickPendingClientMessages();
}

void FKLDebugNetworkArbitrer_Server::TickReadData()
{
    uint32 Size = 0;
    int32 BytesRead = 0;

    while (mListenerSocket->HasPendingData(Size) && Size > 0)
    {
        if (Size > static_cast<uint32>(mReceiveBuffer.Num()))
        {
            mReceiveBuffer.Reset();
            mReceiveBuffer.Init(0, Size);
        }

        mListenerSocket->Recv(mReceiveBuffer.GetData(), Size, BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKL_DebugArbitrer, Warning, TEXT("FKLDebugNetworkArbitrer_Server::TickReadData>> Socket had pending data but didnt read any bytes"));
            continue;
        }

        TArrayView<uint8> ReceiverView{ mReceiveBuffer.GetData(), BytesRead };
        FMemoryReaderView Reader(ReceiverView);
        TickReadData(Reader);
    }
}

void FKLDebugNetworkArbitrer_Server::TickReadData(FArchive& _Reader)
{
    if (_Reader.AtEnd())
    {
        return;
    }

    auto ReadMessagesLambda = [this](const FKLDebugNetworkingMessage_Header& _MessageHeader, FArchive& _MessageData) -> void {
        ReadData(_MessageHeader, _MessageData);
    };

    KL::Debug::Networking::Message::ReadBufferGetStopReadLocation(ReadMessagesLambda, mTempMessageBuffer, _Reader);
}

void FKLDebugNetworkArbitrer_Server::ReadData(const FKLDebugNetworkingMessage_Header& _Header, FArchive& _Reader)
{
    if (_Header.GetMessageEnumType() != static_cast<uint16>(EKLDebugNetworkingMessagesTypes::Arbitrer))
    {
        return;
    }

    switch (static_cast<EKLDebugArbitrerMessage>(_Header.GetMessageType()))
    {
    case EKLDebugArbitrerMessage::ServerPing:
    {
        const FKLDebugNetworkingArbitrerMessage_ServerPing ServerPingMessage{ _Reader };
        const uint32 AnswerPort = ServerPingMessage.Arbitrer_GetAnswerPort();

        bool IsValid = false;
        mTempClientAddress->SetIp(*FIPv4Address::InternalLoopback.ToString(), IsValid);
        ensureMsgf(IsValid, TEXT("address must be valid"));
        mTempClientAddress->SetPort(static_cast<int32>(AnswerPort));

        mTempSendMessageBuffer.Reset();
        mSendMessageBuffer.Reset();
        FMemoryWriter TempWriter{ mTempSendMessageBuffer };
        FMemoryWriter Writer{ mSendMessageBuffer };

        FKLDebugNetworkingArbitrerMessage_ServerPong ServerPongData;
        ServerPongData.Serialize(TempWriter);
        KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerPongData, mTempSendMessageBuffer, Writer);
        int32 BytesSent = 0;
        const bool Sent = mClientSocket->SendTo(mSendMessageBuffer.GetData(), mSendMessageBuffer.Num(), BytesSent, *mTempClientAddress.Get());
        UE_CLOG(!Sent, LogKL_DebugArbitrer, Warning, TEXT("ServerPing>> Coult not send message back to server"));
        break;
    }
    case EKLDebugArbitrerMessage::ServerRegistration:
    {
        const FKLDebugNetworkingArbitrerMessage_ServerConnected ServerMessage{ _Reader };
        const uint32 WorldServerPort = ServerMessage.Arbitrer_GetWorldServerPort();
        const FKLDebugArbitrerDebugServerInfo ServerInfo(ServerMessage.Arbitrer_GetDebugPort(), ServerMessage.Arbitrer_GetHeaderVersion());

        if (FKLDebugArbitrerDebugServerInfo* ServerData = mServerPortToDebugPort.Find(WorldServerPort))
        {
            *ServerData = ServerInfo;
        }
        else
        {
            mServerPortToDebugPort.Emplace(WorldServerPort, ServerInfo);
        }

        UE_LOG(LogKL_DebugArbitrer, Display, TEXT("World server with port [%d] registered with debug port [%d]"), WorldServerPort, ServerMessage.Arbitrer_GetDebugPort());
        break;
    }
    case EKLDebugArbitrerMessage::ServerDisconnect:
    {
        FKLDebugNetworkingArbitrerMessage_ServerDisconnected DisconnectedMessage{ _Reader };
        const uint32 WorldServerPort = DisconnectedMessage.Arbitrer_GetWorldServerPort();
        UE_LOG(LogKL_DebugArbitrer, Display, TEXT("World server with port [%d] uregistered"), WorldServerPort);
        mServerPortToDebugPort.Remove(WorldServerPort);
        break;
    }
    case EKLDebugArbitrerMessage::ClientRegistration:
    {
        mPendingMessages.Emplace(_Reader);
        break;
    }
    default:
        UE_LOG(LogKL_DebugArbitrer, Warning, TEXT("FKLDebugNetworkArbitrer_Server::ReadData>> data type not supported"));
        break;
    }
}

void FKLDebugNetworkArbitrer_Server::TickPendingClientMessages()
{
    if (!mClientSocket || mPendingMessages.IsEmpty())
    {
        return;
    }

    const UKLDebugNetworkingArbitrerSettings& Settings = UKLDebugNetworkingArbitrerSettings::Get();
    for (int32 i = mPendingMessages.Num() - 1; i >= 0; --i)
    {
        const FKLDebugArbitrerPendingMessages& PendingMessage = mPendingMessages[i];
        if (!PendingMessage.GetMessage().Arbitrer_IsValid())
        {
            UE_LOG(LogKL_DebugArbitrer, Error, TEXT("FKLDebugNetworkArbitrer_Server::TickPendingClientMessages>> Client message has invalid host"));
            mPendingMessages.RemoveAtSwap(i, 1, false);
            continue;
        }

        const bool Sent = TrySendPendingClientMessage(PendingMessage.GetMessage());
        if (Sent || PendingMessage.GetTimeSinceAdded() > static_cast<double>(Settings.GetMaxTimeForClientAnswer()))
        {
#if !NO_LOGGING
            const FString ClientHost = mTempClientAddress->ToString(true);
            UE_CLOG(!Sent, LogKL_DebugArbitrer, Error, TEXT("FKLDebugNetworkArbitrer_Server::TickPendingClientMessages>> Coult not send message to client [%s]. Retry time passed"), *ClientHost);
#endif
            mPendingMessages.RemoveAtSwap(i, 1, false);
        }
    }
}

bool FKLDebugNetworkArbitrer_Server::TrySendPendingClientMessage(const FKLDebugNetworkingArbitrerMessage_ClientConnected& _Message)
{
    const FKLDebugArbitrerDebugServerInfo* ServerInfo = mServerPortToDebugPort.Find(_Message.Arbitrer_GetWorldServerPort());
    if (!ServerInfo)
    {
        return false;
    }

    mTempClientAddress->SetIp(_Message.Arbitrer_GetHost());
    mTempSendMessageBuffer.Reset();
    mSendMessageBuffer.Reset();
    FMemoryWriter TempWriter{ mTempSendMessageBuffer };
    FMemoryWriter Writer{ mSendMessageBuffer };

    mTempClientAddress->SetPort(_Message.Arbitrer_GetArbitrerReplyPort());
    const bool CanConnect = ServerInfo->GetHeaderVersion() == _Message.Arbitrer_GetHeaderVersion();
    const uint32 DebugPort = CanConnect ? ServerInfo->GetDebugPort() : 0;

    FKLDebugNetworkingArbitrerMessage_ClientServerData ServerData{ DebugPort, _Message.Arbitrer_GetArbitrerClientID(), CanConnect };
    ServerData.Serialize(TempWriter);
    KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerData, mTempSendMessageBuffer, Writer);
    int32 BytesSent = 0;
    const bool Sent = mClientSocket->SendTo(mSendMessageBuffer.GetData(), mSendMessageBuffer.Num(), BytesSent, *mTempClientAddress.Get());

#if !NO_LOGGING
    const FString ClientHost = mTempClientAddress->ToString(true);
    UE_CLOG(!Sent, LogKL_DebugArbitrer, Warning, TEXT("FKLDebugNetworkArbitrer_Server::HandleClientConnected>> Coult not send message to client [%s]"), *ClientHost);
    UE_CLOG(Sent, LogKL_DebugArbitrer, Display, TEXT("Send message to client [%s]"), *ClientHost);
#endif

    return Sent;
}
