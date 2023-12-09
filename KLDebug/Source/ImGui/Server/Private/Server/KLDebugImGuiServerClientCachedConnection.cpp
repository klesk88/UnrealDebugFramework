// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiServerClientCachedConnection.h"

// modules
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Networking/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerConnected.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Engine/EngineBaseTypes.h"
#include "Engine/NetConnection.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiServerClientCachedConnection::FKLDebugImGuiServerClientCachedConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _ClientSocket)
    : FKLDebugNetworkingCachedConnectionBase(_ReadBufferSize, _WriteBufferSize, _ClientSocket)
    , mClientDataForConnection()
{
}

bool FKLDebugImGuiServerClientCachedConnection::TickOnGameThread(const UWorld& _World)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiTCPServerCachedConnection_TickOnGameThread);

    FSocket& Socket = GetSocketMutable();
    const ESocketConnectionState SocketState = Socket.GetConnectionState();
    if (SocketState != SCS_Connected)
    {
        // waiting to reconnect
        return true;
    }

    // NOTE: when we are inside this call, we are sure that no other operation is performed in this connection as we are locking
    // outside of it
    const UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    const FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainer = ImGuiEngineSubsystem->GetFeatureContainerManager();

    TArray<uint8>& WriteBuffer = GetWriteBuffer();
    FKLDebugNetworkingConnectionGetPendingBuffer PendingMessagesContainer(GetReadBuffer());
    TArray<FKLDebugNetworkingPendingMessage>& ReadBuffer = PendingMessagesContainer.GetPendingMessages();
    FMemoryWriter Writer{ WriteBuffer };

    mClientDataForConnection.ReadData(_World, FeaturesContainer, ReadBuffer);
    mClientDataForConnection.WriteData(_World, FeaturesContainer, Writer);

    return HasNewReadData() || mClientDataForConnection.NeedsTicking();
}
