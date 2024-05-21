// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Selectable/KLDebugFeatureInterface_Selectable.h"
#include "Internal/Networking/KLDebugNetworkingBaseInterface.h"

// engine
#include "Templates/UnrealTypeTraits.h"

class FKLDebugUserNetworkingFeatureSelectableClientTickInput;
class FKLDebugUserNetworkingFeatureSelectableReceiveDataInput;
class FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput;
class FKLDebugUserNetworkingFeatureSelectableServerTickInput;

#define KL_DEBUG_NETWORK_INTERFACE_FEATURE_SELECTABLE(ItemType)                            \
    KL_DEBUG_NETWORK_INTERFACE(ItemType, IKLDebugUserNetworkingFeatureSelectableInterface) \
                                                                                           \
private:    // finish KL_DEBUG_NETWORK_INTERFACE_FEATURE_SELECTABLE

/*
 * Interface that can be implemented on features to allow them to send/receive messages from client server.
 * This should be used ONLY in conjunction with IKLDebugFeatureInterface_Selectable
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_FEATURE_NETWORK_CLASS(ClassType). For example:
 *
 * class FFeatureClassDebug final : public IKLDebugUserNetworkingFeatureSelectableInterface
 * {
 *      KL_DEBUG_NETWORK_INTERFACE_FEATURE_SELECTABLE(FFeatureClassDebug)
 * };
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugUserNetworkingFeatureSelectableInterface : public IKLDebugNetworkingBaseInterface
{
public:
    // server
    [[nodiscard]] virtual bool Server_ShouldTick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) const;
    virtual void Server_Tick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input);
    virtual void Server_FeatureUpdate(const FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput& _Input) const;
    // server

    // client
    virtual void Client_ReceiveData(const FKLDebugUserNetworkingFeatureSelectableReceiveDataInput& _Input);
    virtual void Client_Tick(FKLDebugUserNetworkingFeatureSelectableClientTickInput& _Input);
    // client

    [[nodiscard]] static bool constexpr NetworkShouldPerformStaticChecks();

    template <typename FeatureClass>
    static void constexpr NetworkStaticChecks();

    KL_DEBUG_NETWORK_INTERFACE_INTERNAL_RTTI(IKLDebugUserNetworkingFeatureSelectableInterface, IKLDebugNetworkingBaseInterface)
};

inline bool IKLDebugUserNetworkingFeatureSelectableInterface::Server_ShouldTick([[maybe_unused]] const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) const
{
    checkNoEntry();
    return false;
}

inline void IKLDebugUserNetworkingFeatureSelectableInterface::Server_Tick([[maybe_unused]] const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input)
{
    checkNoEntry();
}

inline void IKLDebugUserNetworkingFeatureSelectableInterface::Server_FeatureUpdate([[maybe_unused]] const FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput& _Input) const
{
    checkNoEntry();
}

inline void IKLDebugUserNetworkingFeatureSelectableInterface::Client_Tick([[maybe_unused]] FKLDebugUserNetworkingFeatureSelectableClientTickInput& _Input)
{
    checkNoEntry();
}

inline void IKLDebugUserNetworkingFeatureSelectableInterface::Client_ReceiveData([[maybe_unused]] const FKLDebugUserNetworkingFeatureSelectableReceiveDataInput& _Input)
{
    checkNoEntry();
}

template <typename FeatureClass>
inline void constexpr IKLDebugUserNetworkingFeatureSelectableInterface::NetworkStaticChecks()
{
    static_assert(TIsDerivedFrom<FeatureClass, IKLDebugFeatureInterface_Selectable>::IsDerived, "IKLDebugUserNetworkingFeatureSelectableInterface can be used only with IKLDebugFeatureInterface_Selectable");
    NetworkPerformStaticChecksInternal<FeatureClass, IKLDebugUserNetworkingFeatureSelectableInterface>();
}

inline bool constexpr IKLDebugUserNetworkingFeatureSelectableInterface::NetworkShouldPerformStaticChecks()
{
    return true;
}
