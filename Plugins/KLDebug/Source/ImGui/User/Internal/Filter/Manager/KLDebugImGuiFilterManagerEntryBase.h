// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryEntryBase.h"

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFilterManagerEntryBase : public TKLDebugUtilsStaticMemoryEntryBase<IKLDebugImGuiFilterInterface, FKLDebugImGuiFilterManagerEntryBase>
{
public:
    explicit FKLDebugImGuiFilterManagerEntryBase(const size_t _ClassSize);
};
