// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Factory/KLDebugNetworkingCommandFactory.h"

// modules
#include "User/Framework/Internal/Networking/Commands/StaticMemory/KLDebugUserNetworkingCommandManager.h"
#include "User/Framework/Internal/Networking/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"
#include "User/Framework/Public/Networking/Commands/Interface/KLDebugUserNetworkingCommandInterface.h"

// engine
#include "Stats/Stats.h"
#include "Stats/Stats2.h"

void FKLDebugNetworkingCommandFactory::Init()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugNetworkingCommandFactory_Init);

    InitInternal<FKLDebugUserNetworkingCommandManager, FKLDebugUserNetworkingCommandManagerEntryBase>();
}
