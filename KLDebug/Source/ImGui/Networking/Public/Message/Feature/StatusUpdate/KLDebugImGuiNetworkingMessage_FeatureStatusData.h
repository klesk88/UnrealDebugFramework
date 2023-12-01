// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "UObject/NameTypes.h"

class FArchive;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_FeatureStatusData
{
public:
    friend FArchive& operator<<(FArchive& Ar, FKLDebugImGuiNetworkingMessage_FeatureStatusData& _FeatureStatusData);

public:
    // explicit FKLDebugImGuiNetworkingMessage_FeatureStatusData(FArchive& _Archive);
    FKLDebugImGuiNetworkingMessage_FeatureStatusData() = default;
    explicit FKLDebugImGuiNetworkingMessage_FeatureStatusData(const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _IsAdded);

    void Serialize(FArchive& _Archive);

    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex Server_GetFeatureIndex() const;
    UE_NODISCARD const FName& Server_GetFeatureNameID() const;
    UE_NODISCARD bool Server_IsAdded() const;

private:
    FName mFeatureNameID;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex;
    bool mIsAdded;
};

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiNetworkingMessage_FeatureStatusData::Server_GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline const FName& FKLDebugImGuiNetworkingMessage_FeatureStatusData::Server_GetFeatureNameID() const
{
    return mFeatureNameID;
}

inline bool FKLDebugImGuiNetworkingMessage_FeatureStatusData::Server_IsAdded() const
{
    return mIsAdded;
}

inline FArchive& operator<<(FArchive& Ar, FKLDebugImGuiNetworkingMessage_FeatureStatusData& _FeatureStatusData)
{
    _FeatureStatusData.Serialize(Ar);
    return Ar;
}