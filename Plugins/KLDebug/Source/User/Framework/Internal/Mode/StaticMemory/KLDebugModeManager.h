// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Mode/StaticMemory/KLDebugModeManagerEntry.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugModeManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugModeManager, FKLDebugModeManagerEntryBase>
{
};
