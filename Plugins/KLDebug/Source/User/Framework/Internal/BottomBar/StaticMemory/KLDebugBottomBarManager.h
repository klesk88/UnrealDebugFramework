// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "BottomBar/StaticMemory/KLDebugBottomBarManagerEntryBase.h"

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugBottomBarManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugBottomBarManager, FKLDebugBottomBarManagerEntryBase>
{
};
