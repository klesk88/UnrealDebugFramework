// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Mode/StaticMemory/KLDebugModeManagerEntryBase.h"
#include "Internal/Networking/KLDebugNetworkingBaseInterface.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryDefines.h"
#include "Mode/KLDebugModeInterface.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "UObject/NameTypes.h"

template <typename ModeInterfaceType>
class TKLDebugModeManagerEntry final : public FKLDebugModeManagerEntryBase
{
public:
    TKLDebugModeManagerEntry(const FName& _NameToCheck);

    // FKLDebugModeManagerEntryBase
    UE_NODISCARD IKLDebugModeInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD SIZE_T GetSize() const final;
    // FKLDebugModeManagerEntryBase
};

template <typename ModeInterfaceType>
TKLDebugModeManagerEntry<ModeInterfaceType>::TKLDebugModeManagerEntry(const FName& _NameToCheck)
    : FKLDebugModeManagerEntryBase(sizeof(ModeInterfaceType))
{
    static_assert(TIsDerivedFrom<ModeInterfaceType, ModeInterfaceType>::IsDerived, "Class passed must derived from IKLDebugModeInterfaceBase");
    checkf(ModeInterfaceType::StaticItemType() == _NameToCheck, TEXT("bottom bar [%s] must define macro KL_DEBUG_DERIVED_MODE in its .h file"), *_NameToCheck.ToString());
    if constexpr (TIsDerivedFrom<ModeInterfaceType, IKLDebugNetworkingBaseInterface>::IsDerived)
    {
        ModeInterfaceType::template NetworkStaticChecks<ModeInterfaceType>();
    }
}

template <typename ModeInterfaceType>
inline IKLDebugModeInterface& TKLDebugModeManagerEntry<ModeInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugModeInterface*>(new (_PoolStartAddress) ModeInterfaceType());
}

template <typename ModeInterfaceType>
inline SIZE_T TKLDebugModeManagerEntry<ModeInterfaceType>::GetSize() const
{
    return sizeof(ModeInterfaceType);
}
