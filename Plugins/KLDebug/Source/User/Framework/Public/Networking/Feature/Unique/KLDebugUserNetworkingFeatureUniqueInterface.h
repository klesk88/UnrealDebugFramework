// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Unique/KLDebugFeatureInterface_Unique.h"
#include "Internal/Networking/KLDebugNetworkingBaseInterface.h"

// c++
#include <type_traits>

class FKLDebugUserNetworkingFeatureUniqueClientTickInput;
class FKLDebugUserNetworkingFeatureUniqueReceiveDataInput;
class FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput;
class FKLDebugUserNetworkingFeatureUniqueServerTickInput;

#define KL_DEBUG_NETWORK_INTERFACE_FEATURE_UNIQUE(ItemType)                            \
    KL_DEBUG_NETWORK_INTERFACE(ItemType, IKLDebugUserNetworkingFeatureUniqueInterface) \
                                                                                       \
private:    // finish KL_DEBUG_NETWORK_INTERFACE_FEATURE_SELECTABLE

/*
 * Interface that can be implemented on features to allow them to send/receive messages from client server
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_FEATURE_NETWORK_CLASS(ClassType). For example:
 *
 * class FFeatureClassDebug final : public IKLDebugUserNetworkingFeatureUniqueInterface
 * {
 *      KL_DEBUG_NETWORK_INTERFACE_FEATURE_UNIQUE(FFeatureClassDebug)
 * };
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugUserNetworkingFeatureUniqueInterface : public IKLDebugNetworkingBaseInterface
{
public:
    // server
    [[nodiscard]] virtual bool Server_ShouldTick(const FKLDebugUserNetworkingFeatureUniqueServerTickInput& _Input) const;
    virtual void Server_Tick(const FKLDebugUserNetworkingFeatureUniqueServerTickInput& _Input);
    virtual void Server_FeatureUpdate(const FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput& _Input) const;
    // server

    // client
    virtual void Client_ReceiveData(const FKLDebugUserNetworkingFeatureUniqueReceiveDataInput& _Input);
    virtual void Client_Tick(FKLDebugUserNetworkingFeatureUniqueClientTickInput& _Input);
    // client

    [[nodiscard]] static bool constexpr NetworkShouldPerformStaticChecks();

    template <typename FeatureClass>
    static void constexpr NetworkStaticChecks();

    KL_DEBUG_NETWORK_INTERFACE_INTERNAL_RTTI(IKLDebugUserNetworkingFeatureUniqueInterface, IKLDebugNetworkingBaseInterface)
};

inline bool IKLDebugUserNetworkingFeatureUniqueInterface::Server_ShouldTick([[maybe_unused]] const FKLDebugUserNetworkingFeatureUniqueServerTickInput& _Input) const
{
    checkNoEntry();
    return false;
}

inline void IKLDebugUserNetworkingFeatureUniqueInterface::Server_Tick([[maybe_unused]] const FKLDebugUserNetworkingFeatureUniqueServerTickInput& _Input)
{
    checkNoEntry();
}

inline void IKLDebugUserNetworkingFeatureUniqueInterface::Server_FeatureUpdate([[maybe_unused]] const FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput& _Input) const
{
    checkNoEntry();
}

inline void IKLDebugUserNetworkingFeatureUniqueInterface::Client_Tick([[maybe_unused]] FKLDebugUserNetworkingFeatureUniqueClientTickInput& _Input)
{
    checkNoEntry();
}

inline void IKLDebugUserNetworkingFeatureUniqueInterface::Client_ReceiveData([[maybe_unused]] const FKLDebugUserNetworkingFeatureUniqueReceiveDataInput& _Input)
{
    checkNoEntry();
}

template <typename FeatureClass>
void constexpr IKLDebugUserNetworkingFeatureUniqueInterface::NetworkStaticChecks()
{
    static_assert(TIsDerivedFrom<FeatureClass, IKLDebugFeatureInterface_Unique>::IsDerived, "IKLDebugUserNetworkingFeatureUniqueInterface can be used only with IKLDebugFeatureInterface_Unique");
    NetworkPerformStaticChecksInternal<FeatureClass, IKLDebugUserNetworkingFeatureUniqueInterface>();
}

inline bool constexpr IKLDebugUserNetworkingFeatureUniqueInterface::NetworkShouldPerformStaticChecks()
{
    return true;
}