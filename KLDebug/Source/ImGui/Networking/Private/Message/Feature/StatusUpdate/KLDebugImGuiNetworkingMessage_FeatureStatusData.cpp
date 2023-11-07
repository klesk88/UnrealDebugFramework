#include "Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusData.h"

//engine
#include "Serialization/Archive.h"

FKLDebugImGuiNetworkingMessage_FeatureStatusData::FKLDebugImGuiNetworkingMessage_FeatureStatusData(const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _IsAdded)
    : mFeatureNameID(_FeatureNameID)
    , mFeatureIndex(_FeatureIndex)
    , mIsAdded(_IsAdded)
{
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusData::Serialize(FArchive& _Archive)
{
    _Archive << mFeatureNameID;
    _Archive << mFeatureIndex;
    _Archive << mIsAdded;
}
