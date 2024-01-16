// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/BottomBar/StaticMemory/KLDebugBottomBarManagerEntryBase.h"
#include "Window/BottomBar/KLDebugBottomBarInterface.h"

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryDefines.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "UObject/NameTypes.h"

class IKLDebugNetworkCheckerInterface;

template <typename BottomBarType>
class TKLDebugBottomBarManagerEntry final : public FKLDebugBottomBarManagerEntryBase
{
public:
    TKLDebugBottomBarManagerEntry(const FName& _NameToCheck);

    // FKLDebugBottomBarManagerEntryBase
    UE_NODISCARD IKLDebugBottomBarInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD SIZE_T GetSize() const final;
    // FKLDebugBottomBarManagerEntryBase
};

template <typename BottomBarType>
TKLDebugBottomBarManagerEntry<BottomBarType>::TKLDebugBottomBarManagerEntry(const FName& _NameToCheck)
    : FKLDebugBottomBarManagerEntryBase(sizeof(BottomBarType))
{
    static_assert(TIsDerivedFrom<BottomBarType, IKLDebugBottomBarInterface>::IsDerived, "Class passed must derived from IKLDebugBottomBarInterface");
    if constexpr (TIsDerivedFrom<BottomBarType, IKLDebugNetworkCheckerInterface>::IsDerived)
    {
        BottomBarType::template NetworkPerformStaticChecks<BottomBarType>();
    }

    checkf(BottomBarType::StaticItemType() == _NameToCheck, TEXT("bottom bar [%s] must define macro KL_DEBUG_DERIVED_BOTTOMBAR in its .h file"), *_NameToCheck.ToString());
}

template <typename BottomBarType>
inline IKLDebugBottomBarInterface& TKLDebugBottomBarManagerEntry<BottomBarType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugBottomBarInterface*>(new (_PoolStartAddress) BottomBarType());
}

template <typename BottomBarType>
inline SIZE_T TKLDebugBottomBarManagerEntry<BottomBarType>::GetSize() const
{
    return sizeof(BottomBarType);
}
