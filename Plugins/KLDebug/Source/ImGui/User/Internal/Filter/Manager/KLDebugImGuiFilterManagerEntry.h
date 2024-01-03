// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Filter/Interface/KLDebugImGuiFilterInterface.h"
#include "Filter/Manager/KLDebugImGuiFilterManagerEntryBase.h"

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryDefines.h"

// engine
#include "Templates/UnrealTypeTraits.h"

// std
#include <cstdint>
#include <type_traits>

class IKLDebugImGuiFeatureInterfaceBase;

GENERATE_STATIC_FUNCTION_CHECK(StaticGetFilterID, const FName&)

template <typename FilterInterfaceType>
class TKLDebugImGuiFilterManagerEntry final : public FKLDebugImGuiFilterManagerEntryBase
{
public:
    TKLDebugImGuiFilterManagerEntry();

    // FKLDebugImGuiFilterManagerEntryBase
    UE_NODISCARD IKLDebugImGuiFilterInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD size_t GetSize() const final;
    // FKLDebugImGuiFilterManagerEntryBase
};

template <typename FilterInterfaceType>
TKLDebugImGuiFilterManagerEntry<FilterInterfaceType>::TKLDebugImGuiFilterManagerEntry()
    : FKLDebugImGuiFilterManagerEntryBase(sizeof(FilterInterfaceType))
{
    static_assert(TIsDerivedFrom<FilterInterfaceType, IKLDebugImGuiFilterInterface>::IsDerived, "Class passed must derived from IKLDebugWindow");
    static_assert(THasStaticFunction_StaticGetFilterID<FilterInterfaceType>::Value, "Class must use the macro KL_DEBUG_FILTER_GENERATE to generate both static and non static methods");
}

template <typename FilterInterfaceType>
IKLDebugImGuiFilterInterface& TKLDebugImGuiFilterManagerEntry<FilterInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugImGuiFilterInterface*>(new (_PoolStartAddress) FilterInterfaceType());
}

template <typename FilterInterfaceType>
size_t TKLDebugImGuiFilterManagerEntry<FilterInterfaceType>::GetSize() const
{
    return sizeof(FilterInterfaceType);
}
