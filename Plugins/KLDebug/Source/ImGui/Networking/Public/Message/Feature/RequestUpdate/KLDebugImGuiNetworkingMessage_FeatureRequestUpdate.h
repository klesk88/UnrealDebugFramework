// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugImGuiNetworkingMessage_Base.h"

// modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "HAL/Platform.h"
#include "Misc/NetworkGuid.h"
#include "UObject/NameTypes.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate final : public TKLDebugImGuiNetworkingMessage_Base<EKLDebugImGuiNetworkMessage::Client_FeatureRequestUpdate>
{
public:
    explicit FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate() = default;
    explicit FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate(const FName& _FeatureName, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const EImGuiInterfaceType _ContainerType, const TArray<uint8>& _BufferData);
    explicit FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate(const FName& _FeatureName, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _ContainerType, const TArray<uint8>& _BufferData);

    UE_NODISCARD const FName& Server_GetFeatureNameID() const;
    UE_NODISCARD const FNetworkGUID& Server_GetFeatureObject() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex Server_GetFeatureIndex() const;
    UE_NODISCARD EImGuiInterfaceType Server_GetContainerType() const;
    UE_NODISCARD TArray<uint8>& Server_GetBufferDataMutable();

private:
    // TKLDebugNetworkingMessage_Base
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // TKLDebugNetworkingMessage_Base

private:
    TArray<uint8> mBufferData;
    FName mFeatureNameID;
    FNetworkGUID mNetworkID;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;
    EImGuiInterfaceType mContainerType = EImGuiInterfaceType::COUNT;
};

inline const FName& FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::Server_GetFeatureNameID() const
{
    return mFeatureNameID;
}

inline const FNetworkGUID& FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::Server_GetFeatureObject() const
{
    return mNetworkID;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::Server_GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline EImGuiInterfaceType FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::Server_GetContainerType() const
{
    return mContainerType;
}

inline TArray<uint8>& FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::Server_GetBufferDataMutable()
{
    return mBufferData;
}