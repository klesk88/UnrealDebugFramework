// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugImGuiNetworkingMessage_Base.h"

// modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"
#include "User/Framework/Public/Networking/Message/KLDebugNetworkingMessageTypes.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Misc/NetworkGuid.h"
#include "UObject/NameTypes.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;
class UWorld;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_FeatureDataUpdate final : public TKLDebugImGuiNetworkingMessage_Base<EKLDebugImGuiNetworkMessage::Server_FeatureDataUpdate>
{
public:
    explicit FKLDebugImGuiNetworkingMessage_FeatureDataUpdate() = default;
    explicit FKLDebugImGuiNetworkingMessage_FeatureDataUpdate(const EKLDebugNetworkReceiveMessageType _ReceiveType, const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _InterfaceType, const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);

    // TKLDebugNetworkingMessage_Base
    [[nodiscard]] bool IsValid() const final;
    // TKLDebugNetworkingMessage_Base

    [[nodiscard]] UObject* Client_GetObjectMutable(const UWorld& _World) const;
    [[nodiscard]] EImGuiInterfaceType Client_GetImGuiInterfaceType() const;
    [[nodiscard]] KL::Debug::ImGui::Features::Types::FeatureIndex Client_GetFeatureIndex() const;
    [[nodiscard]] const FName& Client_GetFeatureNameID() const;
    [[nodiscard]] const TArray<uint8>& Client_GetDataArray() const;
    [[nodiscard]] EKLDebugNetworkReceiveMessageType Client_GetReceiveType() const;

private:
    // TKLDebugNetworkingMessage_Base
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // TKLDebugNetworkingMessage_Base

private:
    TArray<uint8> mFeatureData;
    FNetworkGUID mOwnerObjectNetworkID;
    FName mFeatureNameID;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex;
    EImGuiInterfaceType mInterfaceType = EImGuiInterfaceType::COUNT;
    EKLDebugNetworkReceiveMessageType mReceiveType = EKLDebugNetworkReceiveMessageType::RequestUpdate;
};

inline bool FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::IsValid() const
{
    return TKLDebugImGuiNetworkingMessage_Base<EKLDebugImGuiNetworkMessage::Server_FeatureDataUpdate>::IsValid() && (mInterfaceType == EImGuiInterfaceType::UNIQUE || mOwnerObjectNetworkID.IsValid());
}

inline EImGuiInterfaceType FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::Client_GetImGuiInterfaceType() const
{
    return mInterfaceType;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::Client_GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline const FName& FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::Client_GetFeatureNameID() const
{
    return mFeatureNameID;
}

inline const TArray<uint8>& FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::Client_GetDataArray() const
{
    return mFeatureData;
}

inline EKLDebugNetworkReceiveMessageType FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::Client_GetReceiveType() const
{
    return mReceiveType;
}