// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTypeTraits.h"

// c++
#include <type_traits>

class IKLDebugNetworkingWindowInterface;

#define KL_DEBUG_NETWORK_CHECKER(ItemType, ParentItemType)                                                                        \
public:                                                                                                                           \
    template <typename InterfaceClass>                                                                                            \
    UE_NODISCARD constexpr const InterfaceClass* TryCastToInterface() const                                                       \
    {                                                                                                                             \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                                                        \
        {                                                                                                                         \
            return static_cast<const InterfaceClass*>(this);                                                                      \
        }                                                                                                                         \
                                                                                                                                  \
        return nullptr;                                                                                                           \
    }                                                                                                                             \
                                                                                                                                  \
    template <typename InterfaceClass>                                                                                            \
    UE_NODISCARD constexpr InterfaceClass* TryCastToInterfaceMutable()                                                            \
    {                                                                                                                             \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                                                        \
        {                                                                                                                         \
            return static_cast<InterfaceClass*>(this);                                                                            \
        }                                                                                                                         \
                                                                                                                                  \
        return nullptr;                                                                                                           \
    }                                                                                                                             \
                                                                                                                                  \
    UE_NODISCARD const IKLDebugNetworkingWindowInterface* TryGetNetworkInterface() const override                                 \
    {                                                                                                                             \
        if (const IKLDebugNetworkingWindowInterface* NetworkInterface = TryCastToInterface<IKLDebugNetworkingWindowInterface>())  \
        {                                                                                                                         \
            return NetworkInterface;                                                                                              \
        }                                                                                                                         \
                                                                                                                                  \
        return ParentItemType::TryGetNetworkInterface();                                                                          \
    }                                                                                                                             \
                                                                                                                                  \
    UE_NODISCARD IKLDebugNetworkingWindowInterface* TryGetNetworkInterfaceMutable() override                                      \
    {                                                                                                                             \
        if (IKLDebugNetworkingWindowInterface* NetworkInterface = TryCastToInterfaceMutable<IKLDebugNetworkingWindowInterface>()) \
        {                                                                                                                         \
            return NetworkInterface;                                                                                              \
        }                                                                                                                         \
                                                                                                                                  \
        return ParentItemType::TryGetNetworkInterfaceMutable();                                                                   \
    }                                                                                                                             \
                                                                                                                                  \
private:    // finish macro that allow us to have a basic RTTI system

/*
 */
class IKLDebugNetworkCheckerInterface
{
public:
    virtual ~IKLDebugNetworkCheckerInterface();

    // use the DERIVED_KL_DEBUG_FEATURE_CLASS macro for these
    UE_NODISCARD virtual const IKLDebugNetworkingWindowInterface* TryGetNetworkInterface() const;
    UE_NODISCARD virtual IKLDebugNetworkingWindowInterface* TryGetNetworkInterfaceMutable();
    // DERIVED_KL_DEBUG_FEATURE_CLASS
};

inline const IKLDebugNetworkingWindowInterface* IKLDebugNetworkCheckerInterface::TryGetNetworkInterface() const
{
    return nullptr;
}

inline IKLDebugNetworkingWindowInterface* IKLDebugNetworkCheckerInterface::TryGetNetworkInterfaceMutable()
{
    return nullptr;
}