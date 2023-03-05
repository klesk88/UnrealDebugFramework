#pragma once

#include "StaticMemory/KLDebugImGuiStaticMemoryEntryBase.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFilterManagerEntryBase : public TKLDebugImGuiStaticMemoryEntryBase<IKLDebugImGuiFilterInterface, FKLDebugImGuiFilterManagerEntryBase>
{
public:
    explicit FKLDebugImGuiFilterManagerEntryBase(const size_t _ClassSize);
};
