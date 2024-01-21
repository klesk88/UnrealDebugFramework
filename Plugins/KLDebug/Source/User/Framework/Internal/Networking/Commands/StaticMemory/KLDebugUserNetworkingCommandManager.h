// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingCommandManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugUserNetworkingCommandManager, FKLDebugUserNetworkingCommandManagerEntryBase>
{
};
