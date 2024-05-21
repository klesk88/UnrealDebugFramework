// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusData.h"
#include "Message/KLDebugImGuiNetworkingMessage_Base.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "Misc/NetworkGuid.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate final : public TKLDebugImGuiNetworkingMessage_Base<EKLDebugImGuiNetworkMessage::Client_FeatureStatusUpdate>
{
public:
    explicit FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate() = default;
    explicit FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate(const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _ContainerType);

    // client
    [[nodiscard]] bool Client_IsEqual(const EImGuiInterfaceType _ContainerType, const FNetworkGUID& _NetworkID) const;
    void Client_AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID, const bool _Status);
    void Client_SetFullyRemoved();
    void Client_ClearFullyRemoved();
    // client

    // server
    [[nodiscard]] bool Server_IsFullyRemoved() const;
    [[nodiscard]] const FNetworkGUID& Server_GetNetworkID() const;
    [[nodiscard]] EImGuiInterfaceType Server_GetContainerType() const;
    [[nodiscard]] const TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusData>& Server_GetFeaturesData() const;
    // server

private:
    // TKLDebugNetworkingMessage_Base
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // TKLDebugNetworkingMessage_Base

    void SerializeSelectable(FArchive& _Archive);
    void SerializeUnique(FArchive& _Archive);

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusData> mFeatureData;
    FNetworkGUID mNetworkID;
    bool mFullyRemoved = false;
    EImGuiInterfaceType mContainerType = EImGuiInterfaceType::COUNT;
};

inline void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID, const bool _Status)
{
    mFeatureData.Emplace(_FeatureNameID, _FeatureIndex, _Status);
}

inline bool FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_IsEqual(const EImGuiInterfaceType _ContainerType, const FNetworkGUID& _NetworkID) const
{
    return mContainerType == _ContainerType && mNetworkID == _NetworkID;
}

inline void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_SetFullyRemoved()
{
    mFullyRemoved = true;
}

inline void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_ClearFullyRemoved()
{
    mFullyRemoved = false;
}

inline bool FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_IsFullyRemoved() const
{
    return mFullyRemoved;
}

inline const FNetworkGUID& FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_GetNetworkID() const
{
    return mNetworkID;
}

inline EImGuiInterfaceType FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_GetContainerType() const
{
    return mContainerType;
}

inline const TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusData>& FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_GetFeaturesData() const
{
    return mFeatureData;
}
