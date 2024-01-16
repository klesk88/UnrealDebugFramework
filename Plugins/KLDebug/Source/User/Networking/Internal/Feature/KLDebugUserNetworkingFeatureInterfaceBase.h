// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"

// c++
#include <type_traits>

/*
 * Base network interface. User should never extend directly this one but instead use KLDebugUserNetworkingFeatureUniqueInterface or KLDebugUserNetworkingFeatureSelectableInterface
 */
class KLDEBUGUSERNETWORKING_API IKLDebugUserNetworkingFeatureInterfaceBase
{
public:
    virtual ~IKLDebugUserNetworkingFeatureInterfaceBase();

    // if true we will check the crc of the data buffer gather trough GatherData before sending the message
    // if is the same then we will skip the message
    UE_NODISCARD virtual bool Server_ShouldVerifyCRCBeforeSendData() const;

    // implemented by the macro KL_DEBUG_FEATURE_NETWORK_CLASS
    UE_NODISCARD virtual bool RequireServerTick() const;
    UE_NODISCARD virtual bool RequireClientTick() const;
    // KL_DEBUG_FEATURE_NETWORK_CLASS

protected:
    template <typename FeatureClass, typename InterfaceType>
    static void constexpr NetworkPerformStaticChecksInternal();
};

inline bool IKLDebugUserNetworkingFeatureInterfaceBase::Server_ShouldVerifyCRCBeforeSendData() const
{
    return false;
}

inline bool IKLDebugUserNetworkingFeatureInterfaceBase::RequireServerTick() const
{
    checkNoEntry();
    return false;
}

inline bool IKLDebugUserNetworkingFeatureInterfaceBase::RequireClientTick() const
{
    checkNoEntry();
    return false;
}

template <typename FeatureClass, typename InterfaceType>
void constexpr IKLDebugUserNetworkingFeatureInterfaceBase::NetworkPerformStaticChecksInternal()
{
    static_assert(!std::is_same_v<decltype(&InterfaceType::RequireServerTick), decltype(&FeatureClass::RequireServerTick)>, "feature that extends NetworkingFeatureInterface must also use the appropriate macro in its.h file. Look at comment in the class NetworkingFeatureInterface");
    static_assert(FeatureClass::template NetworkIsSameFeatureType<FeatureClass>(), "feature that extends NetworkingFeatureInterface has a wrong class type passed to the appropriate macro");
    static_assert((std::is_same_v<decltype(&InterfaceType::Server_Tick), decltype(&FeatureClass::Server_Tick)> && std::is_same_v<decltype(&InterfaceType::Server_FeatureUpdate), decltype(&FeatureClass::Server_FeatureUpdate)>) || !std::is_same_v<decltype(&InterfaceType::Client_ReceiveData), decltype(&FeatureClass::Client_ReceiveData)>,
        "feature that extends NetworkingFeatureInterface and implements Server_Tick or Server_FeatureUpdate must also implement Client_ReceiveData so the client can read the data sent by the server");
}
