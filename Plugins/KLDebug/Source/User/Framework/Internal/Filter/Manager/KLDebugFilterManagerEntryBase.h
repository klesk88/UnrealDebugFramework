// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryEntryBase.h"

class IKLDebugFilterInterface;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFilterManagerEntryBase : public TKLDebugUtilsStaticMemoryEntryBase<IKLDebugFilterInterface, FKLDebugFilterManagerEntryBase>
{
public:
    explicit FKLDebugFilterManagerEntryBase(const SIZE_T _ClassSize);
};
