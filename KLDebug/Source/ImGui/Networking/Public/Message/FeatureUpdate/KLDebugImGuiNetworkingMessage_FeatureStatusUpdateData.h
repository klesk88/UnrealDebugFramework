#pragma once

//modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

//engine
#include "UObject/NameTypes.h"

class FArchive;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData
{
public:
    explicit FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData() = default;
    explicit FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData(const FName& _FeatureNameID, const  KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _IsAdded);

    void Write(FArchive& _BitWriter);
    void Read(FArchive& _BitReader);

    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex Server_GetFeatureIndex() const;
    UE_NODISCARD const FName& Server_GetFeatureNameID() const;
    UE_NODISCARD bool Server_IsAdded() const;

private:
    FName mFeatureNameID;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex;
    bool mIsAdded;
};

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData::Server_GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline const FName& FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData::Server_GetFeatureNameID() const
{
    return mFeatureNameID;
}

inline bool FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData::Server_IsAdded() const
{
    return mIsAdded;
}
