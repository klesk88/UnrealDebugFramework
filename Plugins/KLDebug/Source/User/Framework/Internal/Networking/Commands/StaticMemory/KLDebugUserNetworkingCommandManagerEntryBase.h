// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryEntryBase.h"

class IKLDebugUserNetworkingCommandInterface;

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingCommandManagerEntryBase : public TKLDebugUtilsStaticMemoryEntryBase<IKLDebugUserNetworkingCommandInterface, FKLDebugUserNetworkingCommandManagerEntryBase>
{
public:
    explicit FKLDebugUserNetworkingCommandManagerEntryBase(const SIZE_T _ClassSize);
};
