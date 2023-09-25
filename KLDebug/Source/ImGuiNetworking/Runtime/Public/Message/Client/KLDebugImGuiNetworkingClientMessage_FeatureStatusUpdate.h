#pragma once

#include "Message/KLDebugImGuiNetworkingMessageBase.h"

//imgui module
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

//engine
#include "Containers/Map.h"
#include "Misc/NetworkGuid.h"

class UWorld;

class KLDEBUGIMGUINETWORKINGRUNTIME_API FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate final : public FKLDebugImGuiNetworkingMessageBase
{
public:
    using FeatureData = TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, bool>;

public:
    explicit FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate() = default;
    explicit FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate(const FNetworkGUID& _NetworkID);

    //FKLDebugImGuiNetworkingMessageBase
    UE_NODISCARD EKLDebugNetworkMessageTypes GetMessageType() const final;
    UE_NODISCARD bool CanWrite(const UWorld& _World) const final;
    void WriteChild(const UWorld& _World, FBitWriter& _BitWriter) final;
    void ReadChild(const UWorld& _World, FBitReader& _BitReader) final;
    //FKLDebugImGuiNetworkingMessageBase

    //client
    UE_NODISCARD bool Client_IsEqual(const EContainerType _ContainerType, const FNetworkGUID& _NetworkID) const;
    void Client_AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _Status);
    void Client_SetFullyRemoved();
    void Client_ClearFullyRemoved();
    //client

    //server
    UE_NODISCARD bool Server_IsFullyRemoved() const;
    UE_NODISCARD const FNetworkGUID& Server_GetNetworkID() const;
    UE_NODISCARD EContainerType Server_GetContainerType() const;
    UE_NODISCARD const TArray<FeatureData>& Server_GetFeaturesData() const;
    //server

private:
    TArray<FeatureData> mFeatureData;
    FNetworkGUID mNetworkID;
    bool mFullyRemoved = false;
    EContainerType mContainerType = EContainerType::COUNT;
};

inline EKLDebugNetworkMessageTypes FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::GetMessageType() const
{
    return EKLDebugNetworkMessageTypes::Client_FeatureStatusUpdate;
}

inline void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Client_AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _Status)
{
    mFeatureData.Emplace(_FeatureIndex, _Status);
}

inline bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::CanWrite(const UWorld& _World) const
{
    return mNetworkID.IsValid();
}

inline bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Client_IsEqual(const EContainerType _ContainerType, const FNetworkGUID& _NetworkID) const
{
    return mContainerType == _ContainerType && mNetworkID == _NetworkID;
}

inline void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Client_SetFullyRemoved()
{
    mFullyRemoved = true;
}

inline void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Client_ClearFullyRemoved()
{
    mFullyRemoved = false;
}

inline bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Server_IsFullyRemoved() const
{
    return mFullyRemoved;
}

inline const FNetworkGUID& FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Server_GetNetworkID() const
{
    return mNetworkID;
}

inline EContainerType FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Server_GetContainerType() const
{
    return mContainerType;
}

inline const TArray<FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::FeatureData>& FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::Server_GetFeaturesData() const
{
    return mFeatureData;
}
