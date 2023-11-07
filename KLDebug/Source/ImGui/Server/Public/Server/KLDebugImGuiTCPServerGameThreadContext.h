#pragma once

//modules
#include "ImGui/Networking/Public/Subsystem/Engine/KLDebugImGuiNetworkingGameThreadUpdateContextBase.h"

//engine
#include "Containers/Array.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class APlayerController;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiTCPServerGameThreadContext final : public FKLDebugImGuiNetworkingGameThreadUpdateContextBase
{
public:
    explicit FKLDebugImGuiTCPServerGameThreadContext(TArray<TWeakObjectPtr<const APlayerController>>& _NewConnectedPlayers, TArray<FObjectKey>& _DisconnectedPlayers);

    UE_NODISCARD TArray<TWeakObjectPtr<const APlayerController>>& GetNewConnectedPlayers() const;
    UE_NODISCARD TArray<FObjectKey>& GetDisconnectedPlayers() const;

private:
    TArray<TWeakObjectPtr<const APlayerController>>& mConnectedPlayer;
    TArray<FObjectKey>& mDisconnectedPlayers;
};

inline TArray<TWeakObjectPtr<const APlayerController>>& FKLDebugImGuiTCPServerGameThreadContext::GetNewConnectedPlayers() const
{
    return mConnectedPlayer;
}

inline TArray<FObjectKey>& FKLDebugImGuiTCPServerGameThreadContext::GetDisconnectedPlayers() const
{
    return mDisconnectedPlayers;
}
