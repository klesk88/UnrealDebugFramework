// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientWorldCacheConnection.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"

// modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Arbitrer/Public/Messages/Client/KLDebugNetworkingArbitrerMessage_ClientConnected.h"
#include "Networking/Arbitrer/Public/Messages/Client/KLDebugNetworkingArbitrerMessage_ClientServerData.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Common/TcpSocketBuilder.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "HAL/PlatformTime.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Math/NumericLimits.h"
#include "Serialization/Archive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Templates/SharedPointer.h"

namespace KL::Debug::NetworkingClient
{
    static uint16 MessageID = 0;

    UE_NODISCARD constexpr uint16 GetNewID()
    {
        MessageID = static_cast<uint16>((static_cast<uint32>(MessageID) + 1) % TNumericLimits<uint16>::Max());
        return MessageID;
    }
}    // namespace KL::Debug::NetworkingClient

/////////////////////////////////////////////////////////////////////////////////////////////////

FKLDebugImGuiClientWorldCacheConnection::FKLDebugImGuiClientWorldCacheConnection(const UWorld& _World, const FString& _Host, const uint32 _WorldPort, TSharedRef<FInternetAddr> _ArbitrerAddress, FSocket& _ArbitrerSocket)
    : mWorldKey(&_World)
    , mHost(_Host)
    , mArbitrerAddress(_ArbitrerAddress)
    , mArbitrerSocket(&_ArbitrerSocket)
    , mWorldPort(_WorldPort)
    , mID(KL::Debug::NetworkingClient::GetNewID())
{
}

FKLDebugImGuiClientWorldCacheConnection::~FKLDebugImGuiClientWorldCacheConnection()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (SocketSubsystem && mArbitrerSocket)
    {
        mArbitrerSocket->Close();
        SocketSubsystem->DestroySocket(mArbitrerSocket);
        mArbitrerSocket = nullptr;
    }
}

bool FKLDebugImGuiClientWorldCacheConnection::Parallel_Tick(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer)
{
    TickArbitrer(_ArbitrerReplyPort, _WriteTempBuffer, _WriteBuffer);

    if (mCacheConnection.IsValid())
    {
        mCacheConnection->Tick();
        return mCacheConnection->HasNewReadData();
    }

    return false;
}

void FKLDebugImGuiClientWorldCacheConnection::TickArbitrer(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer)
{
    switch (mState)
    {
    case EClientState::PendingSendMessageArbitrer:
    {
        _WriteTempBuffer.Reset();
        _WriteBuffer.Reset();
        FMemoryWriter TempWriter{ _WriteTempBuffer };
        FMemoryWriter Writer{ _WriteBuffer };
        FKLDebugNetworkingArbitrerMessage_ClientConnected Message{ mHost, mWorldPort, _ArbitrerReplyPort, mID };
        Message.Serialize(TempWriter);

        KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(Message, _WriteTempBuffer, Writer);
        int32 ByteSent = 0;
        if (mArbitrerSocket->SendTo(_WriteBuffer.GetData(), _WriteBuffer.Num(), ByteSent, *mArbitrerAddress))
        {
            mState = EClientState::HasSentMessageToArbitrer;
            mSentArbitrerMessageTime = FPlatformTime::Seconds();
        }
        break;
    }
    case EClientState::HasSentMessageToArbitrer:
    {
        const double TimePassed = FPlatformTime::Seconds() - mSentArbitrerMessageTime;
        if (TimePassed > 8.f)
        {
            mState = EClientState::PendingSendMessageArbitrer;
        }
        break;
    }
    case EClientState::MessageReceveid:
        break;
    }
}

void FKLDebugImGuiClientWorldCacheConnection::OnArbitrerMessageRecv(const UKLDebugImGuiNetworkingSettings& _Settings, const FKLDebugNetworkingArbitrerMessage_ClientServerData& _Data, FInternetAddr& _TempAddress)
{
    if (mState == EClientState::MessageReceveid)
    {
        // we receive the message again
        return;
    }

    mState = EClientState::MessageReceveid;

    bool IsValid = false;
    _TempAddress.SetIp(*mHost, IsValid);
    if (!IsValid)
    {
        ensureMsgf(false, TEXT("should be able to parse teh host"));
        return;
    }

    _TempAddress.SetPort(_Data.Client_GetDebugPort());
    FSocket* NewSocket = FTcpSocketBuilder(TEXT("ClientDebugSocket"))
                         .AsNonBlocking()
                         .Build();

    if (!NewSocket)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiClientWorldCacheConnection::OnArbitrerMessageRecv>> Could not create socket"));
        return;
    }

    int32 ReceiveBufferSize = 0;
    int32 SenderBufferSize = 0;
    NewSocket->SetReceiveBufferSize(static_cast<int32>(_Settings.Server_GetReadBufferSize()), ReceiveBufferSize);
    NewSocket->SetSendBufferSize(static_cast<int32>(_Settings.Server_GetWriteBufferSize()), SenderBufferSize);

    if (!NewSocket->Connect(_TempAddress))
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiClientWorldCacheConnection::OnArbitrerMessageRecv>> Could not connect to server"));
        return;
    }

    mCacheConnection = MakeUnique<FKLDebugImGuiClientServerCacheConnection>(ReceiveBufferSize, SenderBufferSize, *NewSocket);
}

bool FKLDebugImGuiClientWorldCacheConnection::TickOnGameThread(FKLDebugImGuiClientData& _ClientData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiTCPClientCachedConnection_TickOnGameThread);

    const UWorld* World = Cast<const UWorld>(mWorldKey.ResolveObjectPtr());
    if (!World)
    {
        ensureMsgf(false, TEXT("world should be valid"));
        return false;
    }

    if (mCacheConnection.IsValid())
    {
        return mCacheConnection->TickOnGameThread(*World, _ClientData);
    }

    return false;
}
