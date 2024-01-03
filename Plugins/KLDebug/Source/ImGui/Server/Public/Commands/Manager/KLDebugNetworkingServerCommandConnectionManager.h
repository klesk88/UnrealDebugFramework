// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Commands/Manager/KLDebugNetworkingCommandConnectionManagerBase.h"

// engine
#include "Containers/Array.h"

class APlayerController;
class FKLDebugNetworkingMessage_Command;
class FKLDebugUserNetworkingCommandExecuteInput_Server;
class IKLDebugUserNetworkingCommandInterface;
class UWorld;

struct FObjectKey;

class KLDEBUGIMGUISERVER_API FKLDebugNetworkingServerCommandConnectionManager final : public FKLDebugNetworkingCommandConnectionManagerBase
{
public:
    // FKLDebugNetworkingCommandConnectionManagerBase
    void AddSendCommand(FKLDebugNetworkingMessage_Command&& _NewCommand) final;
    // FKLDebugNetworkingCommandConnectionManagerBase

    void GameThread_Tick(const TArray<FKLDebugNetworkingCommandConnectionManagerBase*>& _ClientsConnected, UWorld& _CurrentWorld, APlayerController& _PlayerOwner);

    void ClearMessages();

private:
    void PreExecute(const FKLDebugNetworkingMessage_Command& _CommandRcv, const IKLDebugUserNetworkingCommandInterface& _Command, const TArray<FKLDebugNetworkingCommandConnectionManagerBase*>& _ClientsConnected) const;
    void Execute(const FKLDebugUserNetworkingCommandExecuteInput_Server& _Input, const IKLDebugUserNetworkingCommandInterface& _Command, FKLDebugNetworkingMessage_Command& _CommandRcv) const;
};

inline void FKLDebugNetworkingServerCommandConnectionManager::ClearMessages()
{
    mPendingCommandsReceived.Reset();
}