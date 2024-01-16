// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/KLDebugUserNetworkingFeatureInterfaceBase.h"

// modules
#include "User/Framework/Public/Mode/KLDebugModeInterface.h"
#include "User/Framework/Public/Window/BottomBar/KLDebugBottomBarInterface.h"

// c++
#include <type_traits>

class FKLDebugNetworkingWindowClientTickInput;
class FKLDebugNetworkingWindowReceiveDataInput;
class FKLDebugNetworkingWindowRequestUpdateInput;
class FKLDebugNetworkingWindowServerTickInput;

#define KL_DEBUG_NETWORK_WINDOW_CLASS(FeatureType)                                                                              \
public:                                                                                                                         \
    UE_NODISCARD inline bool RequireServerTick() const override                                                                 \
    {                                                                                                                           \
        return !std::is_same_v<decltype(&IKLDebugNetworkingWindowInterface::Server_Tick), decltype(&FeatureType::Server_Tick)>; \
    }                                                                                                                           \
                                                                                                                                \
    UE_NODISCARD inline bool RequireClientTick() const override                                                                 \
    {                                                                                                                           \
        return !std::is_same_v<decltype(&IKLDebugNetworkingWindowInterface::Client_Tick), decltype(&FeatureType::Client_Tick)>; \
    }                                                                                                                           \
                                                                                                                                \
    template <typename FeatureTestType>                                                                                         \
    UE_NODISCARD static bool constexpr NetworkIsSameFeatureType()                                                               \
    {                                                                                                                           \
        return std::is_same_v<FeatureType, FeatureTestType>;                                                                    \
    }                                                                                                                           \
                                                                                                                                \
private:    // finish KL_DEBUG_FEATURE_NETWORK_CLASS

/*
 * Interface that can be implemented on features to allow them to send/receive messages from client server
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_FEATURE_NETWORK_CLASS(ClassType). For example:
 *
 * class FFeatureClassDebug final : public IKLDebugUserNetworkingFeatureUniqueInterface
 * {
 *      KL_DEBUG_FEATURE_NETWORK_UNIQUE_CLASS(FFeatureClassDebug)
 * };
 */
class KLDEBUGUSERNETWORKING_API IKLDebugNetworkingWindowInterface : public IKLDebugUserNetworkingFeatureInterfaceBase
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

    template <typename FeatureClass>
    static void constexpr NetworkPerformStaticChecks();
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
void constexpr IKLDebugNetworkingWindowInterface::NetworkPerformStaticChecks()
{
    static_assert(TIsDerivedFrom<FeatureClass, IKLDebugBottomBarInterface>::Value || TIsDerivedFrom<FeatureClass, IKLDebugModeInterface>::Value, "IKLDebugNetworkingWindowInterface can be used only with IKLDebugBottomBarInterface or IKLDebugModeInterface");
    NetworkPerformStaticChecksInternal<FeatureClass, IKLDebugNetworkingWindowInterface>();
}
