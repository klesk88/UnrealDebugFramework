// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Arbitrer/KLDebugImGuiServerArbitrerManager.h"

// modules
#include "Networking/Arbitrer/Public/Messages/KLDebugNetworkingArbitrerMessage_Types.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerConnected.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerDisconnected.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerPing.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerPong.h"
#include "Networking/Arbitrer/Public/Settings/KLDebugNetworkingArbitrerSettings.h"
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Common/UdpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/EngineBaseTypes.h"
#include "HAL/PlatformTime.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

void FKLDebugImGuiServerArbitrerManager::Init()
{
    if (mArbitrerSenderSocket != nullptr)
    {
        ensureMsgf(false, TEXT("we should never reach here twice"));
        return;
    }

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    const UKLDebugNetworkingArbitrerSettings& ArbitrerSettings = UKLDebugNetworkingArbitrerSettings::Get();

    mArbitrerTempMessageBuffer.Reserve(30);
    mArbitrerTempBuffer.Reserve(500);

    CreateSenderSocket(ArbitrerSettings, *SocketSubsystem);
    CreateListenerSocket(ArbitrerSettings, *SocketSubsystem);

    mTotalTimer = FPlatformTime::Seconds();

    // This is important that is at the end. The code that call Init (from the engine subsystem)
    // can run in parallel with the code that calls method here from the server code.
    // The code on the server tough check this boolean before do anything so is important this is set last
    // so at most the parallel code will skip one frame and perform the operation the next but will
    // not access code that is half initialized
    mHasBeenInitialized = true;
}

void FKLDebugImGuiServerArbitrerManager::Shutdown()
{
    if (mArbitrerSenderSocket || mArbitrerReceiveSocket)
    {
        Clear();
    }
}

void FKLDebugImGuiServerArbitrerManager::Clear()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        // we are shutting down the application
        return;
    }

    if (mArbitrerSenderSocket)
    {
        mArbitrerSenderSocket->Close();
        SocketSubsystem->DestroySocket(mArbitrerSenderSocket);
        mArbitrerSenderSocket = nullptr;
    }

    if (mArbitrerReceiveSocket)
    {
        mArbitrerReceiveSocket->Close();
        SocketSubsystem->DestroySocket(mArbitrerReceiveSocket);
        mArbitrerReceiveSocket = nullptr;
    }

    mArbitrerTempMessageBuffer.Empty();
    mArbitrerTempBuffer.Empty();
    mArbitrerAddress.Reset();
}

void FKLDebugImGuiServerArbitrerManager::CreateSenderSocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings, ISocketSubsystem& _SocketSubsytem)
{
    mArbitrerSenderSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocketSend"))
                                .AsNonBlocking()
                                .AsReusable()
                                .Build();

    if (!mArbitrerSenderSocket)
    {
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::CreateSenderSocket>> Failed to create socket"));
        return;
    }

    mArbitrerTempMessageBuffer.Reserve(30);
    mArbitrerTempBuffer.Reserve(500);

    mArbitrerAddress = _SocketSubsytem.CreateInternetAddr();
    KL::Debug::Networking::Helpers::SetAddressToLocal(mArbitrerAddress.ToSharedRef());
    mArbitrerAddress->SetPort(_ArbitrerSettings.GetPort());

#if !NO_LOGGING
    const FString ConnectionInfo = mArbitrerAddress->ToString(true);
    UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiNetworkingTCPServer::CreateSenderSocket>> Server connecting to arbitrer [%s]"), *ConnectionInfo);
#endif
}

void FKLDebugImGuiServerArbitrerManager::CreateListenerSocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings, ISocketSubsystem& _SocketSubsytem)
{
    uint32 ListeningPort = _ArbitrerSettings.GetServerPortListeningStart();
    while (!mArbitrerReceiveSocket && mArbitrerAnswerPort != _ArbitrerSettings.GetServerPortListeningEnd())
    {
        mArbitrerAnswerPort = ListeningPort++;
        const FIPv4Endpoint Endpoint(FIPv4Address::Any, static_cast<int32>(mArbitrerAnswerPort));
        mArbitrerReceiveSocket = FUdpSocketBuilder(TEXT("NetworkArbitrer_ServerSocket"))
                                     .AsNonBlocking()
                                     .BoundToEndpoint(Endpoint)
                                     .Build();
    }

    if (!mArbitrerReceiveSocket)
    {
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::CreateListenerSocket>> Failed to create socket"));
        return;
    }

    int32 ReceiveBufferSize = 0;
    int32 SenderBufferSize = 0;
    mArbitrerReceiveSocket->SetReceiveBufferSize(static_cast<int32>(_ArbitrerSettings.GetReceiveBufferSize()), ReceiveBufferSize);
    mArbitrerReceiveSocket->SetSendBufferSize(static_cast<int32>(_ArbitrerSettings.GetWriteBufferSize()), SenderBufferSize);
}

void FKLDebugImGuiServerArbitrerManager::ArbitrerAddWorldConnection(const int32 _ServerPort, const int32 _DebugPort)
{
    if (mState == EServerArbitrerState::Failed)
    {
        return;
    }

    mArbitrerTempMessageBuffer.Reset();
    FMemoryWriter ArbitrerTempWriter{ mArbitrerTempMessageBuffer };
    FMemoryWriter ArbitrerWriter{ mArbitrerTempBuffer, false, true };

    FKLDebugNetworkingArbitrerMessage_ServerConnected ServerMessage{ static_cast<uint32>(_ServerPort), static_cast<uint32>(_DebugPort) };
    ServerMessage.Serialize(ArbitrerTempWriter);
    KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerMessage, mArbitrerTempMessageBuffer, ArbitrerWriter);
}

void FKLDebugImGuiServerArbitrerManager::ArbitrerRemovedWorldConnection(const int32 _ServerPort)
{
    if (mState == EServerArbitrerState::Failed)
    {
        return;
    }

    mArbitrerTempMessageBuffer.Reset();
    FMemoryWriter ArbitrerTempWriter{ mArbitrerTempMessageBuffer };
    FMemoryWriter ArbitrerWriter{ mArbitrerTempBuffer, false, true };

    FKLDebugNetworkingArbitrerMessage_ServerDisconnected ServerMessage{ static_cast<uint32>(_ServerPort) };
    ServerMessage.Serialize(ArbitrerTempWriter);
    KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerMessage, mArbitrerTempMessageBuffer, ArbitrerWriter);
}

void FKLDebugImGuiServerArbitrerManager::Parallel_Tick()
{
    if (mState == EServerArbitrerState::Failed)
    {
        return;
    }

    if (!mArbitrerSenderSocket)
    {
        ensureMsgf(false, TEXT("should be valid"));
        return;
    }

    switch (mState)
    {
    case EServerArbitrerState::PendingConnection:
        HandlePendingConnection();
        break;
    case EServerArbitrerState::PendingAnswer:
        HandlePendingAnswer();
        break;
    case EServerArbitrerState::Connected:
        HandleConnected();
        break;
    }
}

void FKLDebugImGuiServerArbitrerManager::HandlePendingConnection()
{
    // try to communicate first with the arbitrer. Even if we are launching locally, depending on the machine, it can take a bit of time before
    // the service is fully up and running. If we just start to send message straight away the service can still be on the startup process and
    // we loose those messages. Make the server firt do a ping/pong type of thing to make sure the arbitrer is ready and then start to send messages
    // to it

    TArray<uint8> ArbitrerTempMessageBuffer;
    TArray<uint8> ArbitrerTempBuffer;

    ArbitrerTempMessageBuffer.Reserve(10);
    ArbitrerTempBuffer.Reserve(60);

    FMemoryWriter ArbitrerTempWriter{ ArbitrerTempMessageBuffer };
    FMemoryWriter Writer{ ArbitrerTempBuffer };

    FKLDebugNetworkingArbitrerMessage_ServerPing ServerMessage{ mArbitrerAnswerPort };
    ServerMessage.Serialize(ArbitrerTempWriter);
    KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerMessage, ArbitrerTempMessageBuffer, Writer);

    SendMessage(ArbitrerTempBuffer);
    mState = EServerArbitrerState::PendingAnswer;
    mTimer = FPlatformTime::Seconds();
}

void FKLDebugImGuiServerArbitrerManager::HandlePendingAnswer()
{
    ReadArbitrerData();

    if (mState != EServerArbitrerState::PendingAnswer)
    {
        return;
    }

    const double TimeSeconds = FPlatformTime::Seconds();
    if (TimeSeconds - mTotalTimer > 15.0)
    {
        Clear();
        mState = EServerArbitrerState::Failed;
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiServerArbitrerManager::HandlePendingAnswer>> Failed to find arbitrer after 15 seconds. Stop retrying"));
    }
    else if ((TimeSeconds - mTimer) > 3.0)
    {
        mState = EServerArbitrerState::PendingConnection;
    }
}

void FKLDebugImGuiServerArbitrerManager::ReadArbitrerData()
{
    uint32 Size = 0;
    int32 BytesRead = 0;

    while (mArbitrerReceiveSocket && mArbitrerReceiveSocket->HasPendingData(Size) && Size > 0)
    {
        TArray<uint8> ReadData;
        ReadData.SetNumUninitialized(Size);
        Size = 0;
        mArbitrerReceiveSocket->Recv(ReadData.GetData(), ReadData.Num(), BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugImGuiServerArbitrerManager::ReadArbitrerData>> Socket had pending data but didnt read any bytes"));
            continue;
        }

        TArrayView<uint8> ReceiverView{ ReadData.GetData(), BytesRead };
        FMemoryReaderView Reader{ ReceiverView };
        bool Success = false;

        auto ReadMessagesLambda = [&Success](const FKLDebugNetworkingMessage_Header& _MessageHeader, FArchive& _MessageData) -> void {
            if (_MessageHeader.GetMessageEnumType() != static_cast<uint16>(EKLDebugNetworkingMessagesTypes::Arbitrer))
            {
                ensureMsgf(false, TEXT("received a message which was not expected"));
                return;
            }

            int32 Index = INDEX_NONE;
            switch (static_cast<EKLDebugArbitrerMessage>(_MessageHeader.GetMessageType()))
            {
            case EKLDebugArbitrerMessage::ServerPong:
            {
                const FKLDebugNetworkingArbitrerMessage_ServerPong ServerData{ _MessageData };
                Success = true;
                break;
            }
            default:
                ensureMsgf(false, TEXT("not handled"));
                break;
            }
        };

        TArray<uint8> ReadMessageData;
        ReadMessageData.SetNumUninitialized(30);
        KL::Debug::Networking::Message::ReadBufferGetStopReadLocation(ReadMessagesLambda, ReadMessageData, Reader);

        if (Success)
        {
            mState = EServerArbitrerState::Connected;
        }
    }
}

void FKLDebugImGuiServerArbitrerManager::SendMessage(TArray<uint8>& _SendeBuffer)
{
    if (_SendeBuffer.IsEmpty())
    {
        return;
    }

    if (!mArbitrerAddress.IsValid())
    {
        ensureMsgf(false, TEXT("Arbitrer addr must be valid if socket is valid"));
        return;
    }

    int32 DataSent = 0;
    if (mArbitrerSenderSocket->SendTo(_SendeBuffer.GetData(), _SendeBuffer.Num(), DataSent, *mArbitrerAddress))
    {
        _SendeBuffer.Reset();
    }
    else
    {
        // we dont expect this. Arbitrer and server machine should live in the same enviroment (same PC for example) so the message
        // should be always be sent correctly.
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiNetworkingTCPServer::NewWorldConnectionAdded>> Failed tosend arbitrer message."));
    }
}
