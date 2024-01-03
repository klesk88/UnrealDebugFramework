// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugImGuiFeatureManager, FKLDebugImGuiFeatureManagerEntryBase>
{
};
