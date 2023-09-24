#pragma once

#include "Message/KLDebugImGuiNetworkingMessageBase.h"

//imgui module
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

//engine
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class UWorld;

class KLDEBUGIMGUINETWORKINGRUNTIME_API FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate final : public FKLDebugImGuiNetworkingMessageBase
{
private:
    using FeatureData = TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, bool>;

public:
    explicit FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate() = default;
    explicit FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate(const AActor& _Object);
    UE_NODISCARD bool operator==(const  AActor& _Object) const;

    //FKLDebugImGuiNetworkingMessageBase
    UE_NODISCARD EKLDebugNetworkMessageTypes GetMessageType() const final;
    UE_NODISCARD bool CanWrite(const UWorld& _World) const final;
    void WriteChild(const UWorld& _World, FBitWriter& _BitWriter) final;
    void ReadChild(const UWorld& _World, FBitReader& _BitReader) final;
    //FKLDebugImGuiNetworkingMessageBase

    void AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _Status);

private:
    TWeakObjectPtr<const UObject> mOwnerObject;
    TArray<FeatureData> mFeatureData;
    EContainerType mContainerType = EContainerType::COUNT;
};

inline EKLDebugNetworkMessageTypes FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::GetMessageType() const
{
    return EKLDebugNetworkMessageTypes::Client_FeatureStatusUpdate;
}

inline void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _Status)
{
    mFeatureData.Emplace(_FeatureIndex, _Status);
}