// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Filter/Interface/KLDebugImGuiFilterInterface.h"
#include "Filter/Manager/KLDebugImGuiFilterManagerEntryBase.h"

// engine
#include "Templates/UnrealTypeTraits.h"

// std
#include <cstdint>
#include <type_traits>

class IKLDebugImGuiFeatureInterfaceBase;

#define GENERATE_STATIC_FUNCTION_CHECK(_FunctionName, _Result, ...) \
    template <typename T>                                           \
    class THasStaticFunction_##_FunctionName                        \
    {                                                               \
        template <typename U, _Result (*)(__VA_ARGS__)>             \
        struct Check;                                               \
        template <typename U>                                       \
        static char StaticTest(Check<U, &T::_FunctionName>*);       \
        template <typename U>                                       \
        static int32 StaticTest(...);                               \
                                                                    \
    public:                                                         \
        enum                                                        \
        {                                                           \
            Value = sizeof(StaticTest<T>(nullptr)) == sizeof(char)  \
        };                                                          \
    };

GENERATE_STATIC_FUNCTION_CHECK(StaticGetFilterID, const FName&)

template <typename FilterInterfaceType>
class TKLDebugImGuiFilterManagerEntry final : public FKLDebugImGuiFilterManagerEntryBase
{
public:
    TKLDebugImGuiFilterManagerEntry();

    // FKLDebugWindowManagerEntryBase
    UE_NODISCARD IKLDebugImGuiFilterInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD size_t GetSize() const final;
    // FKLDebugWindowManagerEntryBase
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
