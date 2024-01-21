// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/KLDebugNetworkingBaseInterface.h"
#include "Mode/KLDebugModeInterface.h"
#include "Window/BottomBar/KLDebugBottomBarInterface.h"

// c++
#include <type_traits>

class FKLDebugNetworkingWindowClientTickInput;
class FKLDebugNetworkingWindowReceiveDataInput;
class FKLDebugNetworkingWindowRequestUpdateInput;
class FKLDebugNetworkingWindowServerTickInput;

#define KL_DEBUG_NETWORK_INTERFACE_WINDOW(ItemType)                         \
    KL_DEBUG_NETWORK_INTERFACE(ItemType, IKLDebugNetworkingWindowInterface) \
                                                                            \
private:    // finish KL_DEBUG_NETWORK_INTERFACE_WINDOW

/*
 * Interface that can be implemented on classes that extends the IKLDebugBottomBarInterface or the  IKLDebugModeInterface.
 * It allows them to communicate over network.
 * IMPORTANT: inside your class definition please use the macro KL_DEBUG_NETWORK_INTERFACE_WINDOW(ClassType). For example:
 *
 * class FFeatureClassDebug final : public IKLDebugUserNetworkingFeatureUniqueInterface
 * {
 *      KL_DEBUG_NETWORK_INTERFACE_WINDOW(FFeatureClassDebug)
 * };
 *
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugNetworkingWindowInterface : public IKLDebugNetworkingBaseInterface
{
public:
    // server
    virtual void Server_Tick(const FKLDebugNetworkingWindowServerTickInput& _Input);
    virtual void Server_FeatureUpdate(const FKLDebugNetworkingWindowRequestUpdateInput& _Input) const;
    // server

    // client
    virtual void Client_ReceiveData(const FKLDebugNetworkingWindowReceiveDataInput& _Input);
    virtual void Client_Tick(FKLDebugNetworkingWindowClientTickInput& _Input);
    // client

    UE_NODISCARD static bool constexpr NetworkShouldPerformStaticChecks();

    template <typename FeatureClass>
    static void constexpr NetworkStaticChecks();

    KL_DEBUG_NETWORK_INTERFACE_INTERNAL_RTTI(IKLDebugNetworkingWindowInterface, IKLDebugNetworkingBaseInterface)
};

inline void IKLDebugNetworkingWindowInterface::Server_Tick([[maybe_unused]] const FKLDebugNetworkingWindowServerTickInput& _Input)
{
    checkNoEntry();
}

inline void IKLDebugNetworkingWindowInterface::Server_FeatureUpdate([[maybe_unused]] const FKLDebugNetworkingWindowRequestUpdateInput& _Input) const
{
    checkNoEntry();
}

inline void IKLDebugNetworkingWindowInterface::Client_Tick([[maybe_unused]] FKLDebugNetworkingWindowClientTickInput& _Input)
{
    checkNoEntry();
}

inline void IKLDebugNetworkingWindowInterface::Client_ReceiveData([[maybe_unused]] const FKLDebugNetworkingWindowReceiveDataInput& _Input)
{
    checkNoEntry();
}

template <typename FeatureClass>
void constexpr IKLDebugNetworkingWindowInterface::NetworkStaticChecks()
{
    static_assert(TIsDerivedFrom<FeatureClass, IKLDebugBottomBarInterface>::IsDerived || TIsDerivedFrom<FeatureClass, IKLDebugModeInterface>::IsDerived, "IKLDebugNetworkingWindowInterface can be used only with IKLDebugBottomBarInterface or IKLDebugModeInterface");
    NetworkPerformStaticChecksInternal<FeatureClass, IKLDebugNetworkingWindowInterface>();
}

inline bool constexpr IKLDebugNetworkingWindowInterface::NetworkShouldPerformStaticChecks()
{
    return true;
}