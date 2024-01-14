// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Factory/KLDebugNetworkingCommandFactory.h"

// modules
#include "User/Networking/Internal/Commands/StaticMemory/KLDebugUserNetworkingCommandManager.h"
#include "User/Networking/Internal/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"
#include "User/Networking/Public/Commands/Interface/KLDebugUserNetworkingCommandInterface.h"

void FKLDebugNetworkingCommandFactory::Init()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugNetworkingCommandFactory_Init);

    InitInternal<FKLDebugUserNetworkingCommandManager, FKLDebugUserNetworkingCommandManagerEntryBase>();
}
