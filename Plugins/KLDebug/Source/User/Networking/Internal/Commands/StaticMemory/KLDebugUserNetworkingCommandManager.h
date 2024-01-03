// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingCommandManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugUserNetworkingCommandManager, FKLDebugUserNetworkingCommandManagerEntryBase>
{
};
