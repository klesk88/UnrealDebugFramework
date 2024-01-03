// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiServerClientCachedConnection.h"

// modules
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Networking/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"
#include "ImGui/Networking/Public/Message/Client/KLDebugNetworkingMessage_ClientConnected.h"
#include "ImGui/Networking/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerConnected.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingMessage.h"

// engine
#include "Engine/EngineBaseTypes.h"
#include "Engine/GameInstance.h"
#include "Engine/NetConnection.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiServerClientCachedConnection::FKLDebugImGuiServerClientCachedConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _ClientSocket)
    : FKLDebugNetworkingCachedConnectionBase(_ReadBufferSize, _WriteBufferSize, _ClientSocket)
{
}

void FKLDebugImGuiServerClientCachedConnection::Parallel_HandlePendingMessageChild(FKLDebugNetworkingPendingMessage&& _PendingMessage)
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
        case EKLDebugNetworkingMessagesTypes::NetworkingRuntime:
            if (_PendingMessage.GetMessageType() == static_cast<uint16>(EKLDebugNetworkingMessage::ClientConnected))
            {
                FMemoryReader Reader{ _PendingMessage.GetMessageData() };
                const FKLDebugNetworkingMessage_ClientConnected Message{ Reader };
                mLocalPlayerNetID = Message.GetPlayerNetID();
                ensureMsgf(!mLocalPlayerNetID.IsValid() || mLocalPlayerNetID == Message.GetPlayerNetID(), TEXT("should not be valid or be the same"));
            }
            else
            {
                UE_LOG(LogKLDebug_Networking, Warning, TEXT("KLDebugImGuiServerClientCachedConnection::Parallel_HandlePendingMessageChild>> Received NetworkingRuntime message which we cant handle"));
            }
            break;
        default:
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("KLDebugImGuiServerClientCachedConnection::Parallel_HandlePendingMessageChild>> Received message which we cant handle"));
            break;
        }
    }
    else
    {
        switch (static_cast<EKLDebugImGuiNetworkMessageTypes>(_PendingMessage.GetMessageEnumType()))
        {
        case EKLDebugImGuiNetworkMessageTypes::ImGuiMessage:
            mClientDataForConnection.AddPendingMessage(MoveTemp(_PendingMessage));
            break;
        default:
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("KLDebugImGuiServerClientCachedConnection::Parallel_HandlePendingMessageChild>> Received message which we cant handle"));
            break;
        }
    }
}

bool FKLDebugImGuiServerClientCachedConnection::TickOnGameThread(const TArray<FKLDebugNetworkingCommandConnectionManagerBase*>& _ClientsConnected, UWorld& _World)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiTCPServerCachedConnection_TickOnGameThread);

    CacheLocalPlayerCharacter(_World);

    FSocket& Socket = GetSocketMutable();
    const ESocketConnectionState SocketState = Socket.GetConnectionState();
    if (SocketState != SCS_Connected)
    {
        // waiting to reconnect
        return true;
    }

    if (APlayerController* PlayerOwner = TryGetPlayerControllerMutable())
    {
        mCommandsManager.GameThread_Tick(_ClientsConnected, _World, *PlayerOwner);
    }
    else
    {
        UE_LOG(LogKLDebug_Networking, Warning, TEXT("KLDebugImGuiServerClientCachedConnection::TickOnGameThread>> Could not find local player owner. Clear any comamnds left"));
        mCommandsManager.ClearMessages();
    }

    // NOTE: when we are inside this call, we are sure that no other operation is performed in this connection as we are locking
    // outside of it
    const UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    const FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainer = ImGuiEngineSubsystem->GetFeatureContainerManager();

    TArray<uint8>& WriteBuffer = GetWriteBuffer();
    FMemoryWriter Writer{ WriteBuffer };

    mClientDataForConnection.ReadData(_World, FeaturesContainer);
    mClientDataForConnection.WriteData(_World, FeaturesContainer, Writer);

    return RequiresGameThreadTick();
}

APlayerController* FKLDebugImGuiServerClientCachedConnection::TryGetPlayerControllerMutable() const
{
    return mLocalPlayerCharacter.Get();
}

const APlayerController* FKLDebugImGuiServerClientCachedConnection::TryGetPlayerController() const
{
    return mLocalPlayerCharacter.Get();
}

void FKLDebugImGuiServerClientCachedConnection::CacheLocalPlayerCharacter(const UWorld& _World)
{
    if (!mLocalPlayerCharacter.IsValid() && mLocalPlayerNetID.IsValid())
    {
        for (FConstPlayerControllerIterator It = _World.GetPlayerControllerIterator(); It; ++It)
        {
            const APlayerState* PlayerState = (*It)->GetPlayerState<APlayerState>();
            if (!PlayerState)
            {
                continue;
            }

            const FUniqueNetIdRepl& UniqueID = PlayerState->GetUniqueId();
            if (UniqueID == mLocalPlayerNetID)
            {
                mLocalPlayerCharacter = *It;
                break;
            }
        }
    }
}
