// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Filter/Manager/KLDebugFilterManagerEntryBase.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryDefines.h"

// engine
#include "Templates/UnrealTypeTraits.h"

// std
#include <cstdint>
#include <type_traits>

GENERATE_STATIC_FUNCTION_CHECK(StaticGetFilterID, const FName&)

template <typename FilterInterfaceType>
class TKLDebugFilterManagerEntry final : public FKLDebugFilterManagerEntryBase
{
public:
    TKLDebugFilterManagerEntry();

    // FKLDebugFilterManagerEntryBase
    [[nodiscard]] IKLDebugFilterInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    [[nodiscard]] SIZE_T GetSize() const final;
    // FKLDebugFilterManagerEntryBase
};

template <typename FilterInterfaceType>
TKLDebugFilterManagerEntry<FilterInterfaceType>::TKLDebugFilterManagerEntry()
    : FKLDebugFilterManagerEntryBase(sizeof(FilterInterfaceType))
{
    static_assert(TIsDerivedFrom<FilterInterfaceType, IKLDebugFilterInterface>::IsDerived, "Class passed must derived from IKLDebugWindow");
    static_assert(THasStaticFunction_StaticGetFilterID<FilterInterfaceType>::Value, "Class must use the macro KL_DEBUG_FILTER_GENERATE to generate both static and non static methods");
}

template <typename FilterInterfaceType>
IKLDebugFilterInterface& TKLDebugFilterManagerEntry<FilterInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugFilterInterface*>(new (_PoolStartAddress) FilterInterfaceType());
}

template <typename FilterInterfaceType>
SIZE_T TKLDebugFilterManagerEntry<FilterInterfaceType>::GetSize() const
{
    return sizeof(FilterInterfaceType);
}
