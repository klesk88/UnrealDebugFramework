// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

// c++
#include <type_traits>

class FArchive;
class FKLDebugImGuiFeature_NetworkingGatherDataInput;
class FKLDebugImGuiFeature_NetworkingReceiveDataInput;
class FKLDebugImGuiFeature_NetworkingTickInput;
class FKLDebugNetworkingFeature_RequestUpdateInput;

#define KL_DEBUG_FEATURE_NETWORK_CLASS(FeatureType)                                                                                    \
public:                                                                                                                                \
    UE_NODISCARD inline bool RequireServerTick() const override                                                                        \
    {                                                                                                                                  \
        return !std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::Server_Tick), decltype(&FeatureType::Server_Tick)>; \
    }                                                                                                                                  \
                                                                                                                                       \
    UE_NODISCARD inline bool RequireClientTick() const override                                                                        \
    {                                                                                                                                  \
        return !std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::Client_Tick), decltype(&FeatureType::Client_Tick)>; \
    }                                                                                                                                  \
                                                                                                                                       \
    template <typename FeatureTestType>                                                                                                \
    UE_NODISCARD static bool constexpr IsSameFeatureType()                                                                             \
    {                                                                                                                                  \
        return std::is_same_v<FeatureType, FeatureTestType>;                                                                           \
    }                                                                                                                                  \
                                                                                                                                       \
private:    // finish KL_DEBUG_FEATURE_NETWORK_CLASS

/*
 * Interface that can be implemented on features to allow them to send/receive messages from client server
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_FEATURE_NETWORK_CLASS(ClassType). For example:
 *
 * class FFeatureClassDebug final : public IKLDebugImGuiFeature_NetworkingInterface
 * {
 *      KL_DEBUG_FEATURE_NETWORK_CLASS(FFeatureClassDebug)
 * };
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeature_NetworkingInterface
{
public:
    virtual ~IKLDebugImGuiFeature_NetworkingInterface();

    UE_NODISCARD virtual bool Server_ShouldTick(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const;
    virtual void Server_Tick(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput);
    virtual void Server_FeatureUpdate(const FKLDebugNetworkingFeature_RequestUpdateInput& _Input) const;
    // if true we will check the crc of the data buffer gather trough GatherData before sending the message
    // if is the same then we will skip the message
    UE_NODISCARD virtual bool Server_ShouldVerifyCRCBeforeSendData() const;

    virtual void Client_ReceiveData(const FKLDebugImGuiFeature_NetworkingReceiveDataInput& _Input);
    virtual void Client_Tick(FKLDebugImGuiFeature_NetworkingTickInput& _Input);

    // implemented by the macro
    UE_NODISCARD virtual bool RequireServerTick() const;
    UE_NODISCARD virtual bool RequireClientTick() const;
    //

    template <typename FeatureClass>
    static void constexpr PerformStaticChecks();
};

inline bool IKLDebugImGuiFeature_NetworkingInterface::Server_ShouldTick([[maybe_unused]] const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const
{
    checkNoEntry();
    return false;
}

inline void IKLDebugImGuiFeature_NetworkingInterface::Server_Tick([[maybe_unused]] const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput)
{
    checkNoEntry();
}

inline bool IKLDebugImGuiFeature_NetworkingInterface::Server_ShouldVerifyCRCBeforeSendData() const
{
    return true;
}

inline void IKLDebugImGuiFeature_NetworkingInterface::Client_Tick([[maybe_unused]] FKLDebugImGuiFeature_NetworkingTickInput& _Input)
{
    checkNoEntry();
}

inline void IKLDebugImGuiFeature_NetworkingInterface::Server_FeatureUpdate([[maybe_unused]] const FKLDebugNetworkingFeature_RequestUpdateInput& _Input) const
{
    checkNoEntry();
}

inline void IKLDebugImGuiFeature_NetworkingInterface::Client_ReceiveData([[maybe_unused]] const FKLDebugImGuiFeature_NetworkingReceiveDataInput& _Input)
{
    checkNoEntry();
}

inline bool IKLDebugImGuiFeature_NetworkingInterface::RequireServerTick() const
{
    checkNoEntry();
    return false;
}

inline bool IKLDebugImGuiFeature_NetworkingInterface::RequireClientTick() const
{
    checkNoEntry();
    return false;
}

template <typename FeatureClass>
void constexpr IKLDebugImGuiFeature_NetworkingInterface::PerformStaticChecks()
{
    static_assert(!std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::RequireServerTick), decltype(&FeatureClass::RequireServerTick)>, "feature that extends IKLDebugImGuiFeature_NetworkingInterface must also use the macro KL_DEBUG_FEATURE_NETWORK_CLASS in its.h file. Look at comment in the class IKLDebugImGuiFeature_NetworkingInterface");
    static_assert(FeatureClass::template IsSameFeatureType<FeatureClass>(), "feature that extends IKLDebugImGuiFeature_NetworkingInterface has a wrong class type passed to the macro KL_DEBUG_FEATURE_NETWORK_CLASS");
    static_assert(std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::Server_Tick), decltype(&FeatureClass::Server_Tick)> || !std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::Server_ShouldTick), decltype(&FeatureClass::Server_ShouldTick)>, "feature that extends IKLDebugImGuiFeature_NetworkingInterface and implements Server_Tick must also implement Server_ShouldTick");
    static_assert((std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::Server_Tick), decltype(&FeatureClass::Server_Tick)> && std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::Server_FeatureUpdate), decltype(&FeatureClass::Server_FeatureUpdate)>) || !std::is_same_v<decltype(&IKLDebugImGuiFeature_NetworkingInterface::Client_ReceiveData), decltype(&FeatureClass::Client_ReceiveData)>,
        "feature that extends IKLDebugImGuiFeature_NetworkingInterface and implements Server_Tick or Server_FeatureUpdate must also implement Client_ReceiveData so the client can read the data sent by the server");
}
