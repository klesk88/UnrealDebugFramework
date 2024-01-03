// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryDefines.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "UObject/NameTypes.h"

class IKLDebugUserNetworkingCommandInterface;

template <typename CommandInterfaceType>
class TKLDebugUserNetworkingCommandManagerEntry final : public FKLDebugUserNetworkingCommandManagerEntryBase
{
public:
    TKLDebugUserNetworkingCommandManagerEntry();

    // FKLDebugImGuiFilterManagerEntryBase
    UE_NODISCARD IKLDebugUserNetworkingCommandInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD SIZE_T GetSize() const final;
    // FKLDebugImGuiFilterManagerEntryBase
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
