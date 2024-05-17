// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Manager/KLDebugNetworkingServerCommandConnectionManager.h"

// modules
#include "ImGui/Networking/Public/Commands/Manager/KLDebugNetworkingCommandManager.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "User/Framework/Public/Networking/Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Server.h"
#include "User/Framework/Public/Networking/Commands/Interface/KLDebugUserNetworkingCommandInterface.h"

// engine
#include "Engine/World.h"
#include "Serialization/MemoryReader.h"

namespace KL::Debug::Server::Command
{
    template <typename FilterCallback>
    void BroadcastCommand(const FKLDebugNetworkingMessage_Command& _CommandRcv, const IKLDebugUserNetworkingCommandInterface& _Command, const TArray<FKLDebugNetworkingCommandConnectionManagerBase*>& _ClientsConnected, const FilterCallback& _Filter)
    {
        for (FKLDebugNetworkingCommandConnectionManagerBase* ClientCommandManager : _ClientsConnected)
        {
            if (_Filter(*ClientCommandManager))
            {
                continue;
            }

            if (_CommandRcv.GetCommandContextData().IsEmpty())
            {
                ClientCommandManager->AddSendCommand(FKLDebugNetworkingMessage_Command{ _CommandRcv.GetCommandID() });
            }
            else
            {
                TArray<uint8> CommandContextData = _CommandRcv.GetCommandContextData();
                ClientCommandManager->AddSendCommand(FKLDebugNetworkingMessage_Command{ _CommandRcv.GetCommandID(), MoveTemp(CommandContextData) });
            }
        }
    }
}    // namespace KL::Debug::Server::Command

/////////////////////////////////////////////////////////////////////////////////////////

void FKLDebugNetworkingServerCommandConnectionManager::AddSendCommand(FKLDebugNetworkingMessage_Command&& _NewCommand)
{
    FKLDebugNetworkingCommandConnectionManagerBase::AddSendCommand(MoveTemp(_NewCommand));
}

void FKLDebugNetworkingServerCommandConnectionManager::GameThread_Tick(const TArray<FKLDebugNetworkingCommandConnectionManagerBase*>& _ClientsConnected, UWorld& _CurrentWorld, APlayerController& _PlayerOwner)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingServerCommandConnectionManager_GTTick);

    const FKLDebugUserNetworkingCommandExecuteInput_Server Input{ _PlayerOwner, _CurrentWorld };

    for (FKLDebugNetworkingMessage_Command& CommandRcv : mPendingCommandsReceived)
    {
        const IKLDebugUserNetworkingCommandInterface* Command = KL::Debug::Networking::Commands::Manager::TryGetCommand(CommandRcv.GetCommandID());
        if (!Command)
        {
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("Command [%s] not found in factory skipping for world [%s]"), *CommandRcv.GetCommandID().ToString(), *_CurrentWorld.GetName());
            continue;
        }

        PreExecute(CommandRcv, *Command, _ClientsConnected);
        Execute(Input, *Command, CommandRcv);
    }

    mPendingCommandsReceived.Reset();
}

void FKLDebugNetworkingServerCommandConnectionManager::PreExecute(const FKLDebugNetworkingMessage_Command& _CommandRcv, const IKLDebugUserNetworkingCommandInterface& _Command, const TArray<FKLDebugNetworkingCommandConnectionManagerBase*>& _ClientsConnected) const
{
    switch (_Command.Server_ShouldExecute())
    {
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_Execute:
        break;
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_ExecuteAndBroadcastSenderIncluded:
    {
        auto NoOpFilter = []([[maybe_unused]] const FKLDebugNetworkingCommandConnectionManagerBase& _CommandManager) -> bool {
            return false;
        };

        KL::Debug::Server::Command::BroadcastCommand(_CommandRcv, _Command, _ClientsConnected, NoOpFilter);
        break;
    }
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_ExecuteAndBroadcast:
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_BroadcastOnly:
    {
        auto FilterLocalClient = [this](const FKLDebugNetworkingCommandConnectionManagerBase& _CommandManager) -> bool {
            return &_CommandManager == this;
        };

        KL::Debug::Server::Command::BroadcastCommand(_CommandRcv, _Command, _ClientsConnected, FilterLocalClient);
        break;
    }
    }
}

void FKLDebugNetworkingServerCommandConnectionManager::Execute(const FKLDebugUserNetworkingCommandExecuteInput_Server& _Input, const IKLDebugUserNetworkingCommandInterface& _Command, FKLDebugNetworkingMessage_Command& _CommandRcv) const
{
    FMemoryReader Reader{ _CommandRcv.GetCommandContextDataMutable() };

    // execute block
    switch (_Command.Server_ShouldExecute())
    {
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_Execute:
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_ExecuteAndBroadcast:
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_ExecuteAndBroadcastSenderIncluded:
        _Command.Server_Execute(_Input, Reader);
        break;
    case IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType::Server_BroadcastOnly:
        break;
    }
}
