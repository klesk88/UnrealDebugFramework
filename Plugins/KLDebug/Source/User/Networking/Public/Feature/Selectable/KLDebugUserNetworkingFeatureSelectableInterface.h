// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/KLDebugUserNetworkingFeatureInterfaceBase.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"

// engine
#include "Templates/UnrealTypeTraits.h"

class FKLDebugUserNetworkingFeatureSelectableClientTickInput;
class FKLDebugUserNetworkingFeatureSelectableReceiveDataInput;
class FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput;
class FKLDebugUserNetworkingFeatureSelectableServerTickInput;

#define KL_DEBUG_FEATURE_NETWORK_SELECTABLE_CLASS(FeatureType)                                                                                 \
public:                                                                                                                                        \
    UE_NODISCARD inline bool RequireServerTick() const override                                                                                \
    {                                                                                                                                          \
        return !std::is_same_v<decltype(&IKLDebugUserNetworkingFeatureSelectableInterface::Server_Tick), decltype(&FeatureType::Server_Tick)>; \
    }                                                                                                                                          \
                                                                                                                                               \
    UE_NODISCARD inline bool RequireClientTick() const override                                                                                \
    {                                                                                                                                          \
        return !std::is_same_v<decltype(&IKLDebugUserNetworkingFeatureSelectableInterface::Client_Tick), decltype(&FeatureType::Client_Tick)>; \
    }                                                                                                                                          \
                                                                                                                                               \
    template <typename FeatureTestType>                                                                                                        \
    UE_NODISCARD static bool constexpr NetworkIsSameFeatureType()                                                                              \
    {                                                                                                                                          \
        return std::is_same_v<FeatureType, FeatureTestType>;                                                                                   \
    }                                                                                                                                          \
                                                                                                                                               \
private:    // finish KL_DEBUG_FEATURE_NETWORK_CLASS

/*
 * Interface that can be implemented on features to allow them to send/receive messages from client server.
 * This should be used ONLY in conjunction with IKLDebugImGuiFeatureInterface_Selectable
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_FEATURE_NETWORK_CLASS(ClassType). For example:
 *
 * class FFeatureClassDebug final : public IKLDebugUserNetworkingFeatureSelectableInterface
 * {
 *      KL_DEBUG_FEATURE_NETWORK_SELECTABLE_CLASS(FFeatureClassDebug)
 * };
 */
class KLDEBUGUSERNETWORKING_API IKLDebugUserNetworkingFeatureSelectableInterface : public IKLDebugUserNetworkingFeatureInterfaceBase
{
public:
    // server
    UE_NODISCARD virtual bool Server_ShouldTick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) const;
    virtual void Server_Tick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input);
    virtual void Server_FeatureUpdate(const FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput& _Input) const;
    // server

    // client
    virtual void Client_ReceiveData(const FKLDebugUserNetworkingFeatureSelectableReceiveDataInput& _Input);
    virtual void Client_Tick(FKLDebugUserNetworkingFeatureSelectableClientTickInput& _Input);
    // client

    // implemented by macro
    UE_NODISCARD virtual bool RequireServerTick() const;
    UE_NODISCARD virtual bool RequireClientTick() const;
    // macro

    template <typename FeatureClass>
    static void constexpr PerformStaticChecks();
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

inline bool IKLDebugUserNetworkingFeatureSelectableInterface::RequireServerTick() const
{
    checkNoEntry();
    return false;
}

inline bool IKLDebugUserNetworkingFeatureSelectableInterface::RequireClientTick() const
{
    checkNoEntry();
    return false;
}

template <typename FeatureClass>
inline void constexpr IKLDebugUserNetworkingFeatureSelectableInterface::PerformStaticChecks()
{
    static_assert(TIsDerivedFrom<FeatureClass, IKLDebugImGuiFeatureInterface_Selectable>::Value, "IKLDebugUserNetworkingFeatureSelectableInterface can be used only with IKLDebugImGuiFeatureInterface_Selectable");
    PerformStaticChecksInternal<FeatureClass, IKLDebugUserNetworkingFeatureSelectableInterface>();
}