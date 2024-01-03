// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Manager/KLDebugNetworkingClientCommandConnectionManager.h"

// modules
#include "ImGui/Networking/Public/Commands/Manager/KLDebugNetworkingCommandManager.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "User/Networking/Internal/Commands/Helpers/KLDebugUserNetworkingCommandsRequester.h"
#include "User/Networking/Public/Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Client.h"
#include "User/Networking/Public/Commands/Interface/KLDebugUserNetworkingCommandInterface.h"

// engine
#include "Serialization/MemoryReader.h"

FKLDebugNetworkingClientCommandConnectionManager::FKLDebugNetworkingClientCommandConnectionManager(const FObjectKey& _WorldKey)
    : mWorldKey(_WorldKey)
{
    KL::Debug::Networking::Commands::Manager::BindOnNewCommandWorldRequest(_WorldKey, FOnKLDebugNewCommandWorld::CreateRaw(this, &FKLDebugNetworkingClientCommandConnectionManager::OnNewCommandRequest));
}

FKLDebugNetworkingClientCommandConnectionManager::~FKLDebugNetworkingClientCommandConnectionManager()
{
    KL::Debug::Networking::Commands::Manager::UnbindOnNewCommandWorldRequest(mWorldKey);
}

void FKLDebugNetworkingClientCommandConnectionManager::OnNewCommandRequest(const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& _Input)
{
#if !NO_LOGGING
    const bool HasValidFilter = static_cast<bool>(_Input.GetClientFilter());
    UE_CLOG(HasValidFilter, LogKLDebug_Networking, Warning, TEXT("FKLDebugNetworkingClientCommandConnectionManager::OnNewCommandRequest>> Using the server version of the AddCommand for a client instance. The filter will not be applied"));
#endif

    if (_Input.GetContextData().IsEmpty())
    {
        mPendingCommandsToSend.Enqueue(FKLDebugNetworkingMessage_Command(_Input.GetCommandID()));
    }
    else
    {
        mPendingCommandsToSend.Enqueue(FKLDebugNetworkingMessage_Command(_Input.GetCommandID(), _Input.GetContextDataMutable()));
    }
}

void FKLDebugNetworkingClientCommandConnectionManager::GameThread_Tick(UWorld& _CurrentWorld)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingClientCommandConnectionManager_GTTick);

    const FKLDebugUserNetworkingCommandExecuteInput_Client Input{ _CurrentWorld };

    for (FKLDebugNetworkingMessage_Command& CommandRcv : mPendingCommandsReceived)
    {
        const IKLDebugUserNetworkingCommandInterface* Command = KL::Debug::Networking::Commands::Manager::TryGetCommand(CommandRcv.GetCommandID());
        if (!Command)
        {
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("KLDebugNetworkingClientCommandConnectionManager::GameThread_Tick>> Command [%s] not found in factory skipping for world [%s]"), *CommandRcv.GetCommandID().ToString(), *_CurrentWorld.GetName());
            continue;
        }

        FMemoryReader Reader{ CommandRcv.GetCommandContextDataMutable() };
        Command->Client_Execute(Input, Reader);
    }

    mPendingCommandsReceived.Reset();
}