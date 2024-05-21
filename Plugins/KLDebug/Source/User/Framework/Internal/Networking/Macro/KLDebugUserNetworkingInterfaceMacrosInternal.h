// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// c++
#include <type_traits>

#define KL_DEBUG_NETWORK_INTERFACE_INTERNAL_RTTI(NetworkInterfaceType, ParentItemType)                              \
public:                                                                                                             \
    [[nodiscard]] inline static const FName& NetworkStaticItemType()                                                 \
    {                                                                                                               \
        static const FName Interface(TEXT(#NetworkInterfaceType));                                                  \
        return Interface;                                                                                           \
    }                                                                                                               \
                                                                                                                    \
protected:                                                                                                          \
    [[nodiscard]] inline bool NetworkIsDerivedInternal(const FName& _ItemTypeName) const override                    \
    {                                                                                                               \
        return NetworkStaticItemType() == _ItemTypeName || ParentItemType::NetworkIsDerivedInternal(_ItemTypeName); \
    }                                                                                                               \
                                                                                                                    \
private:    // finish KL_DEBUG_NETWORK_INTERFACE_INTERNAL_RTTI
