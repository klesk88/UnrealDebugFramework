// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryEntryBase.h"

class IKLDebugModeInterface;

class KLDEBUGUSERFRAMEWORK_API FKLDebugModeManagerEntryBase : public TKLDebugUtilsStaticMemoryEntryBase<IKLDebugModeInterface, FKLDebugModeManagerEntryBase>
{
public:
    explicit FKLDebugModeManagerEntryBase(const SIZE_T _ClassSize);
};
