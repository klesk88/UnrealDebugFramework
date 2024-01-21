// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "BottomBar/StaticMemory/KLDebugBottomBarManagerEntryBase.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugBottomBarManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugBottomBarManager, FKLDebugBottomBarManagerEntryBase>
{
};
