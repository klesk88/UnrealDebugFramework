// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryDefines.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "UObject/NameTypes.h"

class IKLDebugUserNetworkingCommandInterface;

template <typename CommandInterfaceType>
class TKLDebugUserNetworkingCommandManagerEntry final : public FKLDebugUserNetworkingCommandManagerEntryBase
{
public:
    TKLDebugUserNetworkingCommandManagerEntry();

    // FKLDebugFilterManagerEntryBase
    [[nodiscard]] IKLDebugUserNetworkingCommandInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    [[nodiscard]] SIZE_T GetSize() const final;
    // FKLDebugFilterManagerEntryBase
};

template <typename CommandInterfaceType>
TKLDebugUserNetworkingCommandManagerEntry<CommandInterfaceType>::TKLDebugUserNetworkingCommandManagerEntry()
    : FKLDebugUserNetworkingCommandManagerEntryBase(sizeof(CommandInterfaceType))
{
    static_assert(TIsDerivedFrom<CommandInterfaceType, IKLDebugUserNetworkingCommandInterface>::IsDerived, "Class passed must derived from IKLDebugWindow");
}

template <typename CommandInterfaceType>
inline IKLDebugUserNetworkingCommandInterface& TKLDebugUserNetworkingCommandManagerEntry<CommandInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<CommandInterfaceType*>(new (_PoolStartAddress) CommandInterfaceType());
}

template <typename CommandInterfaceType>
inline SIZE_T TKLDebugUserNetworkingCommandManagerEntry<CommandInterfaceType>::GetSize() const
{
    return sizeof(CommandInterfaceType);
}
