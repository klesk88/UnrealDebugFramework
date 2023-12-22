// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ServerPortGetter/Arbitrer/KLDebugImGuiClientArbitrerCacheConnection.h"

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

FKLDebugImGuiClientArbitrerCacheConnection::FKLDebugImGuiClientArbitrerCacheConnection(const UWorld& _World, const uint32 _WorldPort, TSharedRef<FInternetAddr> _ArbitrerAddress, FSocket& _ArbitrerSocket)
    : mWorldKey(&_World)
    , mArbitrerAddress(_ArbitrerAddress)
    , mArbitrerSocket(&_ArbitrerSocket)
    , mWorldPort(_WorldPort)
    , mID(KL::Debug::NetworkingClient::GetNewID())
{
    _ArbitrerAddress->GetIp(mHost);
}

FKLDebugImGuiClientArbitrerCacheConnection::~FKLDebugImGuiClientArbitrerCacheConnection()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (SocketSubsystem && mArbitrerSocket)
    {
        mArbitrerSocket->Close();
        SocketSubsystem->DestroySocket(mArbitrerSocket);
        mArbitrerSocket = nullptr;
    }
}

void FKLDebugImGuiClientArbitrerCacheConnection::Parallel_Tick(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer)
{
    TickArbitrer(_ArbitrerReplyPort, _WriteTempBuffer, _WriteBuffer);
}

void FKLDebugImGuiClientArbitrerCacheConnection::TickArbitrer(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer)
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
