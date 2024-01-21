// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Networking/Commands/Interface/KLDebugUserNetworkingCommandInterface.h"

class FKLDebugUserNetworkingCommandExecuteInput_Client;
class FKLDebugUserNetworkingCommandExecuteInput_Server;

class KLDEBUGEXAMPLES_API FKLDebugCommand_Pause final : public IKLDebugUserNetworkingCommandInterface
{
    KL_DEBUG_COMMAND_GENERATE_WITHOUT_CONTEXT(FKLDebugCommand_Pause)

public:
    void Client_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Client& _Input) const;
    void Server_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Server& _Input) const;
};

inline void FKLDebugCommand_Pause::Client_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Client& _Input) const
{
}