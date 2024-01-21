// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Macro/KLDebugUserNetworkingInterfaceMacrosInternal.h"

#define KL_DEBUG_NETWORK_INTERFACE(ItemType, NetworkInterfaceType)                                              \
public:                                                                                                         \
    UE_NODISCARD inline bool RequireServerTick() const override                                                 \
    {                                                                                                           \
        return !std::is_same_v<decltype(&NetworkInterfaceType::Server_Tick), decltype(&ItemType::Server_Tick)>; \
    }                                                                                                           \
                                                                                                                \
    UE_NODISCARD inline bool RequireClientTick() const override                                                 \
    {                                                                                                           \
        return !std::is_same_v<decltype(&NetworkInterfaceType::Client_Tick), decltype(&ItemType::Client_Tick)>; \
    }                                                                                                           \
                                                                                                                \
private:    // finish KL_DEBUG_NETWORK_INTERFACE

/*
 */
class IKLDebugNetworkingBaseInterface
{
public:
    virtual ~IKLDebugNetworkingBaseInterface();

    // RTTI section
    template <typename InterfaceClass>
    UE_NODISCARD const InterfaceClass* NetworkTryCastToInterface() const;
    template <typename InterfaceClass>
    UE_NODISCARD InterfaceClass* NetworkTryCastToInterfaceMutable();

    template <typename OtherType>
    UE_NODISCARD bool NetworkIsDerivedFrom() const;
    // RTTI section

    // if true we will check the crc of the data buffer gather trough GatherData before sending the message
    // if is the same then we will skip the message
    UE_NODISCARD virtual bool Server_ShouldVerifyCRCBeforeSendData() const;

    // implemented by the macro KL_DEBUG_FEATURE_NETWORK_CLASS
    UE_NODISCARD virtual bool RequireServerTick() const;
    UE_NODISCARD virtual bool RequireClientTick() const;
    // KL_DEBUG_FEATURE_NETWORK_CLASS

protected:
protected:
    //  RTTI section these will be extended from the network module by the different network interfaces
    UE_NODISCARD virtual bool NetworkIsDerivedInternal(const FName& _ItemTypeName) const;
    UE_NODISCARD static const FName& NetworkStaticItemType();
    //

    template <typename FeatureClass, typename InterfaceType>
    static void constexpr NetworkPerformStaticChecksInternal();
};

inline bool IKLDebugNetworkingBaseInterface::Server_ShouldVerifyCRCBeforeSendData() const
{
    return false;
}

inline bool IKLDebugNetworkingBaseInterface::RequireServerTick() const
{
    checkNoEntry();
    return false;
}

inline bool IKLDebugNetworkingBaseInterface::RequireClientTick() const
{
    checkNoEntry();
    return false;
}

template <typename ClassType, typename InterfaceType>
void constexpr IKLDebugNetworkingBaseInterface::NetworkPerformStaticChecksInternal()
{
    static_assert(!std::is_same_v<decltype(&InterfaceType::RequireServerTick), decltype(&ClassType::RequireServerTick)>, "feature that extends NetworkingFeatureInterface must also use the appropriate macro in its.h file. Look at comment in the class");
    static_assert((std::is_same_v<decltype(&InterfaceType::Server_Tick), decltype(&ClassType::Server_Tick)> && std::is_same_v<decltype(&InterfaceType::Server_FeatureUpdate), decltype(&ClassType::Server_FeatureUpdate)>) || !std::is_same_v<decltype(&InterfaceType::Client_ReceiveData), decltype(&ClassType::Client_ReceiveData)>,
        "feature that extends NetworkingFeatureInterface and implements Server_Tick or Server_FeatureUpdate must also implement Client_ReceiveData so the client can read the data sent by the server");
}

template <typename OtherType>
inline bool IKLDebugNetworkingBaseInterface::NetworkIsDerivedFrom() const
{
    return NetworkIsDerivedInternal(OtherType::NetworkStaticItemType());
}

template <typename InterfaceClass>
const InterfaceClass* IKLDebugNetworkingBaseInterface::NetworkTryCastToInterface() const
{
    if (NetworkIsDerivedFrom<InterfaceClass>())
    {
        return static_cast<const InterfaceClass*>(this);
    }

    return nullptr;
}

template <typename InterfaceClass>
InterfaceClass* IKLDebugNetworkingBaseInterface::NetworkTryCastToInterfaceMutable()
{
    if (NetworkIsDerivedFrom<InterfaceClass>())
    {
        return static_cast<InterfaceClass*>(this);
    }

    return nullptr;
}

inline bool IKLDebugNetworkingBaseInterface::NetworkIsDerivedInternal(const FName& _ItemTypeName) const
{
    return _ItemTypeName == NetworkStaticItemType();
}

inline const FName& IKLDebugNetworkingBaseInterface::NetworkStaticItemType()
{
    static const FName Interface(TEXT("IKLDebugNetworkingBaseInterface"));
    return Interface;
}
