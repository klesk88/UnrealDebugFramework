// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/KLDebugCommand_Pause.h"

// modules
#include "User/Networking/Public/Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Client.h"
#include "User/Networking/Public/Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Server.h"

// engine
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "User/Networking/Public/Commands/Helpers/KLDebugUserNetworkingCommandHelpers.h"

KL_DEBUG_CREATE_COMMAND(FKLDebugCommand_Pause)

void FKLDebugCommand_Pause::Server_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Server& _Input) const
{
    const bool ShouldPause = !_Input.GetOwningPlayerConnectionMutable().IsPaused();
    _Input.GetOwningPlayerConnectionMutable().SetPause(ShouldPause);
}
