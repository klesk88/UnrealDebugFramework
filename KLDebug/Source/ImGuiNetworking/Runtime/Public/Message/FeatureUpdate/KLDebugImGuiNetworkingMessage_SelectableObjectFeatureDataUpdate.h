#pragma once

#include "Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdateData.h"
#include "Message/KLDebugImGuiNetworkingMessageBase.h"

//modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

//engine
#include "HAL/Platform.h"
#include "Misc/NetworkGuid.h"

class FName;
class UWorld;

class KLDEBUGIMGUINETWORKINGRUNTIME_API FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate final : public FKLDebugImGuiNetworkingMessageBase
{
public:
    explicit FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate() = default;
    explicit FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate(const FNetworkGUID& _NetworkID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const TArray<uint8>& _Data);

    //FKLDebugImGuiNetworkingMessageBase
    UE_NODISCARD EKLDebugNetworkMessageTypes GetMessageType() const final;
    UE_NODISCARD bool CanWrite(const UWorld& _World) const final;
    void WriteChild(const UWorld& _World, FArchive& _BitWriter) final;
    void ReadChild(const UWorld& _World, FArchive& _BitReader) final;
    //FKLDebugImGuiNetworkingMessageBase

    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureIndex() const;
    UE_NODISCARD const UObject* GetObject(const UWorld& _World) const;
    UE_NODISCARD TArray<uint8>& GetDataArray();

private:
    FNetworkGUID mOwnerObjectNetworkID;
    TArray<uint8> mFeatureData;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;
};

inline EKLDebugNetworkMessageTypes FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::GetMessageType() const
{
    return EKLDebugNetworkMessageTypes::Server_FeatureDataUpdate;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline const UObject* FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::GetObject(const UWorld& _World) const
{
    return TryGetObjectFromNetworkGUID(_World, mOwnerObjectNetworkID);
}

inline TArray<uint8>& FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::GetDataArray()
{
    return mFeatureData;
}