// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryEntryBase.h"

class IKLDebugBottomBarInterface;

class KLDEBUGUSERFRAMEWORK_API FKLDebugBottomBarManagerEntryBase : public TKLDebugUtilsStaticMemoryEntryBase<IKLDebugBottomBarInterface, FKLDebugBottomBarManagerEntryBase>
{
public:
    explicit FKLDebugBottomBarManagerEntryBase(const SIZE_T _ClassSize);
};
