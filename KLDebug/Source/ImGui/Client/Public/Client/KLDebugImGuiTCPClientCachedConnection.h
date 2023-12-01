// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientManager.h"

// modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPCachedConnectionBase.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/CriticalSection.h"
#include "HAL/Platform.h"
#include "Misc/DateTime.h"
#include "Misc/NetworkGuid.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FArchive;
class FKLDebugImGuiClientData;
class FKLDebugImGuiNetworkingMessage_Header;
class FSocket;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiTCPClientCachedConnection final : public FKLDebugImGuiNetworkingTCPCachedConnectionBase
{
private:
    enum class EClientState : uint8
    {
        PendingServerData = 0,
        FullyInitialized,

        Failure
    };

public:
    explicit FKLDebugImGuiTCPClientCachedConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket);
    UE_NODISCARD bool operator==(const FNetworkGUID& _ControllerGUID) const;
    UE_NODISCARD bool operator==(const FObjectKey& _WorldID) const;

    // FKLDebugImGuiNetworkingTCPCachedConnectionBase
    UE_NODISCARD bool IsValid() const final;
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase

    UE_NODISCARD bool HasServerInitializedOwner() const;

    UE_NODISCARD bool TickOnGameThread(FKLDebugImGuiClientData& _ClientData);

private:
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase
    UE_NODISCARD bool TickChild() final;
    UE_NODISCARD bool ReadBufferChildHasHandleMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, FArchive& _Data) final;
    void TickChildWriteBuffer(FArchive& _Writer) final;
    // FKLDebugImGuiNetworkingTCPCachedConnectionBase

    void HandlePendingServerDataState();

    UE_NODISCARD bool PendingServerDataStateReadData(const FKLDebugImGuiNetworkingMessage_Header& _Header, FArchive& _Data);

private:
    FKLDebugImGuiClientManager mClientImGuiData;
    FDateTime mCheckTimer;
    FObjectKey mWorldKey;
    FNetworkGUID mControllerOwnerGUID;
    EClientState mState = EClientState::PendingServerData;
    bool mIsWorldValid = true;
};

inline bool FKLDebugImGuiTCPClientCachedConnection::operator==(const FNetworkGUID& _ControllerGUID) const
{
    return _ControllerGUID == mControllerOwnerGUID;
}

inline bool FKLDebugImGuiTCPClientCachedConnection::operator==(const FObjectKey& _WorldID) const
{
    return mWorldKey == _WorldID;
}

inline bool FKLDebugImGuiTCPClientCachedConnection::HasServerInitializedOwner() const
{
    return mControllerOwnerGUID.IsValid();
}