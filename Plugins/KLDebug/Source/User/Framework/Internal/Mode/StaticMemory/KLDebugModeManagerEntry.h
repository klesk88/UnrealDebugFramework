// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Mode/StaticMemory/KLDebugModeManagerEntryBase.h"

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryDefines.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "UObject/NameTypes.h"

class IKLDebugModeInterface;

template <typename ModeInterfaceType>
class TKLDebugModeManagerEntry final : public FKLDebugModeManagerEntryBase
{
public:
    TKLDebugModeManagerEntry();

    // FKLDebugImGuiFilterManagerEntryBase
    UE_NODISCARD IKLDebugModeInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD SIZE_T GetSize() const final;
    // FKLDebugImGuiFilterManagerEntryBase
};

template <typename ModeInterfaceType>
TKLDebugModeManagerEntry<ModeInterfaceType>::TKLDebugModeManagerEntry()
    : FKLDebugModeManagerEntryBase(sizeof(ModeInterfaceType))
{
    static_assert(TIsDerivedFrom<ModeInterfaceType, ModeInterfaceType>::IsDerived, "Class passed must derived from IKLDebugModeInterfaceBase");
}

template <typename ModeInterfaceType>
inline IKLDebugModeInterface& TKLDebugModeManagerEntry<ModeInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<ModeInterfaceType*>(new (_PoolStartAddress) ModeInterfaceType());
}

template <typename ModeInterfaceType>
inline SIZE_T TKLDebugModeManagerEntry<ModeInterfaceType>::GetSize() const
{
    return sizeof(ModeInterfaceType);
}
