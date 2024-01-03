// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Commands/Manager/KLDebugNetworkingServerCommandConnectionManager.h"
#include "Server/KLDebugImGuiServerCacheConnection.h"

// modules
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingCachedConnectionBase.h"

// engine
#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/NetworkGuid.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class APlayerController;
class FArchive;
class FKLDebugNetworkingPendingMessage;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerClientCachedConnection final : public FKLDebugNetworkingCachedConnectionBase
{
public:
    explicit FKLDebugImGuiServerClientCachedConnection(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _ClientSocket);

    // FKLDebugNetworkingCachedConnectionBase
    UE_NODISCARD bool RequiresGameThreadTick() const final;
    // FKLDebugNetworkingCachedConnectionBase

    // this is called from the game thread.
    UE_NODISCARD bool TickOnGameThread(const TArray<FKLDebugNetworkingCommandConnectionManagerBase*>& _ClientsConnected, UWorld& _World);
    UE_NODISCARD APlayerController* TryGetPlayerControllerMutable() const;
    UE_NODISCARD const APlayerController* TryGetPlayerController() const;
    UE_NODISCARD FKLDebugNetworkingServerCommandConnectionManager& GetCommandManagerMutable();

    void AddCommand(FKLDebugNetworkingMessage_Command&& _Command);

private:
    // FKLDebugNetworkingCachedConnectionBase
    void Parallel_HandlePendingMessageChild(FKLDebugNetworkingPendingMessage&& _PendingMessage) final;
    void TickChildWriteBuffer(FArchive& _Writer) final;
    // FKLDebugNetworkingCachedConnectionBase

    void CacheLocalPlayerCharacter(const UWorld& _World);

private:
    FKLDebugImGuiServerCacheConnection mClientDataForConnection;
    FKLDebugNetworkingServerCommandConnectionManager mCommandsManager;
    FNetworkGUID mLocalPlayerID;
    FUniqueNetIdRepl mLocalPlayerNetID;
    TWeakObjectPtr<APlayerController> mLocalPlayerCharacter;
    double mCheckTimer;
};

inline bool FKLDebugImGuiServerClientCachedConnection::RequiresGameThreadTick() const
{
    return mClientDataForConnection.RequiresGameThreadTick() || mCommandsManager.HasPendingData();
}

inline void FKLDebugImGuiServerClientCachedConnection::AddCommand(FKLDebugNetworkingMessage_Command&& _Command)
{
    mCommandsManager.AddSendCommand(MoveTemp(_Command));
}

inline void FKLDebugImGuiServerClientCachedConnection::TickChildWriteBuffer(FArchive& _Writer)
{
    mCommandsManager.Parallel_Tick(_Writer);
}

inline FKLDebugNetworkingServerCommandConnectionManager& FKLDebugImGuiServerClientCachedConnection::GetCommandManagerMutable()
{
    return mCommandsManager;
}