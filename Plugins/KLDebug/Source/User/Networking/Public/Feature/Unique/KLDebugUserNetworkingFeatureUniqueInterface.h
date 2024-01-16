// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/KLDebugUserNetworkingFeatureInterfaceBase.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"

// c++
#include <type_traits>

class FKLDebugUserNetworkingFeatureUniqueClientTickInput;
class FKLDebugUserNetworkingFeatureUniqueReceiveDataInput;
class FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput;
class FKLDebugUserNetworkingFeatureUniqueServerTickInput;

#define KL_DEBUG_FEATURE_NETWORK_UNIQUE_CLASS(FeatureType)                                                                                 \
public:                                                                                                                                    \
    UE_NODISCARD inline bool RequireServerTick() const override                                                                            \
    {                                                                                                                                      \
        return !std::is_same_v<decltype(&IKLDebugUserNetworkingFeatureUniqueInterface::Server_Tick), decltype(&FeatureType::Server_Tick)>; \
    }                                                                                                                                      \
                                                                                                                                           \
    UE_NODISCARD inline bool RequireClientTick() const override                                                                            \
    {                                                                                                                                      \
        return !std::is_same_v<decltype(&IKLDebugUserNetworkingFeatureUniqueInterface::Client_Tick), decltype(&FeatureType::Client_Tick)>; \
    }                                                                                                                                      \
                                                                                                                                           \
    template <typename FeatureTestType>                                                                                                    \
    UE_NODISCARD static bool constexpr NetworkIsSameFeatureType()                                                                          \
    {                                                                                                                                      \
        return std::is_same_v<FeatureType, FeatureTestType>;                                                                               \
    }                                                                                                                                      \
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
class KLDEBUGUSERNETWORKING_API IKLDebugUserNetworkingFeatureUniqueInterface : public IKLDebugUserNetworkingFeatureInterfaceBase
{
public:
    // server
    UE_NODISCARD virtual bool Server_ShouldTick(const FKLDebugUserNetworkingFeatureUniqueServerTickInput& _Input) const;
    virtual void Server_Tick(const FKLDebugUserNetworkingFeatureUniqueServerTickInput& _Input);
    virtual void Server_FeatureUpdate(const FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput& _Input) const;
    // server

    // client
    virtual void Client_ReceiveData(const FKLDebugUserNetworkingFeatureUniqueReceiveDataInput& _Input);
    virtual void Client_Tick(FKLDebugUserNetworkingFeatureUniqueClientTickInput& _Input);
    // client

    template <typename FeatureClass>
    static void constexpr PerformStaticChecks();
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
void constexpr IKLDebugUserNetworkingFeatureUniqueInterface::PerformStaticChecks()
{
    static_assert(TIsDerivedFrom<FeatureClass, IKLDebugImGuiFeatureInterface_Unique>::Value, "IKLDebugUserNetworkingFeatureUniqueInterface can be used only with IKLDebugImGuiFeatureInterface_Unique");
    NetworkPerformStaticChecksInternal<FeatureClass, IKLDebugUserNetworkingFeatureUniqueInterface>();
}
