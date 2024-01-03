// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientServerCacheConnection.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"

// modules
#include "ImGui/Networking/Public/Message/Client/KLDebugNetworkingMessage_ClientConnected.h"
#include "ImGui/Networking/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingMessage.h"

// engine
#include "Engine/World.h"
#include "Serialization/Archive.h"
#include "Serialization/MemoryWriter.h"

FKLDebugImGuiClientServerCacheConnection::FKLDebugImGuiClientServerCacheConnection(const FObjectKey& _WorldKey, const FUniqueNetIdRepl& _LocalPlayerNetID, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket)
    : FKLDebugNetworkingCachedConnectionBase(_ReadBufferSize, _WriteBufferSize, _Socket)
    , mCommandsManager(_WorldKey)
    , mLocalPlayerNetID(_LocalPlayerNetID)
{
}

bool FKLDebugImGuiClientServerCacheConnection::IsValid() const
{
    if (!FKLDebugNetworkingCachedConnectionBase::IsValid())
    {
        return false;
    }

    switch (mState)
    {
    case EClientState::SendClientInit:
    case EClientState::FullyInitialized:
        return true;
    case EClientState::Failure:
        return false;
    }

    return false;
}

void FKLDebugImGuiClientServerCacheConnection::TickChildWriteBuffer(FArchive& _Writer)
{
    switch (mState)
    {
    case EClientState::SendClientInit:
    {
        TArray<uint8> MessageBuffer;
        TArray<uint8> CompressMessageBuffer;
        MessageBuffer.Reserve(30);
        FMemoryWriter MessageWriter{ MessageBuffer };
        FKLDebugNetworkingMessage_ClientConnected ClientMessage{ mLocalPlayerNetID };
        ClientMessage.Serialize(MessageWriter);

        KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ClientMessage, MessageBuffer, _Writer);
        mState = EClientState::FullyInitialized;
    }
    break;
    case EClientState::FullyInitialized:
        mCommandsManager.Parallel_Tick(_Writer);
        mClientImGuiData.Parallel_TickWriteData(_Writer);
        break;
    case EClientState::Failure:
        break;
    }
}

void FKLDebugImGuiClientServerCacheConnection::Parallel_HandlePendingMessageChild(FKLDebugNetworkingPendingMessage&& _PendingMessage)
{
    if (_PendingMessage.GetMessageEnumType() >= static_cast<uint16>(EKLDebugImGuiNetworkMessageTypes::Count))
    {
        ensureMsgf(false, TEXT("could not recognize the message type"));
        return;
    }

    if (_PendingMessage.GetMessageEnumType() < static_cast<uint16>(EKLDebugNetworkingMessagesTypes::Count))
    {
        switch (static_cast<EKLDebugNetworkingMessagesTypes>(_PendingMessage.GetMessageEnumType()))
        {
        case EKLDebugNetworkingMessagesTypes::Command:
            mCommandsManager.AddPendingMessage(MoveTemp(_PendingMessage));
            break;
        default:
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("Received message which we cant handle"));
            break;
        }
    }
    else
    {
        switch (static_cast<EKLDebugImGuiNetworkMessageTypes>(_PendingMessage.GetMessageEnumType()))
        {
        case EKLDebugImGuiNetworkMessageTypes::ImGuiMessage:
            mClientImGuiData.AddPendingMessage(MoveTemp(_PendingMessage));
            break;
        default:
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("Received message which we cant handle"));
            break;
        }
    }
}

bool FKLDebugImGuiClientServerCacheConnection::TickOnGameThread(UWorld& _World, FKLDebugImGuiClientData& _ClientData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiTCPClientCachedConnection_TickOnGameThread);

    TArray<uint8>& WriteBuffer = GetWriteBuffer();

    mCommandsManager.GameThread_Tick(_World);
    mClientImGuiData.GameThread_TickReadData(_ClientData);
    return HasPendingDataToRead();
}