// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Server.h"

FKLDebugUserNetworkingCommandExecuteInput_Server::FKLDebugUserNetworkingCommandExecuteInput_Server(APlayerController& _PlayerClientOwner, UWorld& _World)
    : FKLDebugUserNetworkingCommandExecuteInput_Base(_World)
    , mPlayerClientOwner(_PlayerClientOwner)
{
}
