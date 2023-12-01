// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"

FKLDebugImGuiTCPServerGameThreadContext::FKLDebugImGuiTCPServerGameThreadContext(TArray<TWeakObjectPtr<const APlayerController>>& _NewConnectedPlayers, TArray<FObjectKey>& _DisconnectedPlayers)
    : mConnectedPlayer(_NewConnectedPlayers)
    , mDisconnectedPlayers(_DisconnectedPlayers)
{
}
