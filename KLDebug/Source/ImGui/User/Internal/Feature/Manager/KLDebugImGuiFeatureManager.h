// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"
#include "StaticMemory/KLDebugImGuiStaticMemoryManager.h"

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureManager final : public TKLDebugImGuiStaticMemoryManager<FKLDebugImGuiFeatureManager, FKLDebugImGuiFeatureManagerEntryBase>
{
};
