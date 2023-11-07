#pragma once

#include "Server/KLDebugImGuiServerCacheConnection.h"
#include "Server/KLDebugImGuiServerConnectionDefinitions.h"

//modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPCachedConnectionBase.h"

//engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Misc/DateTime.h"
#include "Misc/NetworkGuid.h"
#include "IPAddress.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AController;
class FArchive;
class FSocket;
class ISocketSubsystem;
class UNetConnection;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiTCPServerCachedConnection final : public FKLDebugImGuiNetworkingTCPCachedConnectionBase
{
private:
    enum class EConnectionState : uint8
    {
        ToInitialize = 0,
        PendingInitializeSend,
        PendingClientConnection,
        ClientConnected,
        Failure,
    };

public:
    explicit FKLDebugImGuiTCPServerCachedConnection(const AController& _OwnerController, const KL::Debug::ImGui::Networking::Server::CacheConnectionID _ID, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _ClientSocket);
    UE_NODISCARD bool operator==(const FSocket& _Socket) const;
    UE_NODISCARD bool operator==(const FObjectKey& _ControllerKey) const;
    UE_NODISCARD bool operator==(const KL::Debug::ImGui::Networking::Server::CacheConnectionID _ID) const;

    UE_NODISCARD KL::Debug::ImGui::Networking::Server::CacheConnectionID GetID() const;
    UE_NODISCARD bool IsPartOfWorld(const FObjectKey& _WorldID) const;

    void SetInvalidConnection();
    UE_NODISCARD bool IsValidConnection() const;
    void OnClientConnected(ISocketSubsystem& _SocketSubsystem, FSocket& _ClientSocket);

    //this is called from the game thread.
    UE_NODISCARD bool TickOnGameThread();

private:
    //FKLDebugImGuiNetworkingTCPCachedConnectionBase
    UE_NODISCARD bool TickChild() final;
    void TickChildWriteBuffer(FArchive& _Writer) final;
    void OnSendDataResultChild(const bool _DataSent) final;
    //FKLDebugImGuiNetworkingTCPCachedConnectionBase

    void HandleToInitializeState();
    void HandlePendingClientConnectionState();
    void HandleClientConnectedState();

    void WriteInitializeState(FArchive& _Writer);

private:
    FKLDebugImGuiServerCacheConnection mClientDataForConnection;
    FDateTime mCheckTimer;
    //store the network IDS so we can access them while sending message
    //in parallel without having issues of safe access
    FNetworkGUID mWorldNetworkKey;
    FNetworkGUID mControllerNetworkKey;
    FObjectKey mControllerKey;
    FObjectKey mWorldKey;
    TWeakObjectPtr<const UWorld> mWorld;

    bool mIsValidConnection = true;
    KL::Debug::ImGui::Networking::Server::CacheConnectionID mID = KL::Debug::ImGui::Networking::Server::InvalidCacheConnectionID;
    EConnectionState mConnectionState = EConnectionState::ToInitialize;
};

inline bool FKLDebugImGuiTCPServerCachedConnection::operator==(const FSocket& _Socket) const
{
    ensureMsgf(false, TEXT("need to checck probably doesnt work"));
    return &GetSocket() == &_Socket;
}

inline bool FKLDebugImGuiTCPServerCachedConnection::operator==(const FObjectKey& _ControllerKey) const
{
    return mControllerKey == _ControllerKey;
}

inline bool FKLDebugImGuiTCPServerCachedConnection::operator==(const KL::Debug::ImGui::Networking::Server::CacheConnectionID _ID) const
{
    return mID == _ID;
}

inline KL::Debug::ImGui::Networking::Server::CacheConnectionID FKLDebugImGuiTCPServerCachedConnection::GetID() const
{
    return mID;
}

inline bool FKLDebugImGuiTCPServerCachedConnection::IsPartOfWorld(const FObjectKey& _WorldID) const
{
    return mWorldKey == _WorldID;
}

inline void FKLDebugImGuiTCPServerCachedConnection::SetInvalidConnection()
{
    mIsValidConnection = false;
}
