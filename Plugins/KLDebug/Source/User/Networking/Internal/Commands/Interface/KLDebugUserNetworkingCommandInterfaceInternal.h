// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Commands/Interface/KLDebugUserNetworkingCommandInterfaceMacros.h"

// engine
#include "CoreMinimal.h"

class FArchive;
class FKLDebugUserNetworkingCommandExecuteInput_Client;
class FKLDebugUserNetworkingCommandExecuteInput_Server;

class KLDEBUGUSERNETWORKING_API IKLDebugUserNetworkingCommandInterfaceInternal
{
public:
    virtual ~IKLDebugUserNetworkingCommandInterfaceInternal() = default;

    // these need to be overridden by using the macro KL_DEBUG_COMMAND_GENERATE_WITHOUT_CONTEXT or KL_DEBUG_COMMAND_GENERATE_WITH_CONTEXT
    virtual void Server_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Server& _Input, FArchive& _ArchiveContext) const = 0;
    virtual void Client_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Client& _Input, FArchive& _ArchiveContext) const = 0;
    UE_NODISCARD virtual const FName& GetCommandID() const = 0;
    // end macro overrides
};
