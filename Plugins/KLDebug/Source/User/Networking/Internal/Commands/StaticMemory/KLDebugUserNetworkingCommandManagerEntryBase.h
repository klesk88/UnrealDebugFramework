// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryEntryBase.h"

class IKLDebugUserNetworkingCommandInterface;

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingCommandManagerEntryBase : public TKLDebugUtilsStaticMemoryEntryBase<IKLDebugUserNetworkingCommandInterface, FKLDebugUserNetworkingCommandManagerEntryBase>
{
public:
    explicit FKLDebugUserNetworkingCommandManagerEntryBase(const SIZE_T _ClassSize);
};
