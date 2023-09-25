#include "Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdateData.h"

//engine
#include "Serialization/BitReader.h"
#include "Serialization/BitWriter.h"

FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData::FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData(const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _IsAdded)
    : mFeatureNameID(_FeatureNameID)
    , mFeatureIndex(_FeatureIndex)
    , mIsAdded(_IsAdded)
{
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData::Write(FBitWriter& _BitWriter)
{
    _BitWriter << mFeatureNameID;
    _BitWriter << mFeatureIndex;
    _BitWriter << mIsAdded;
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData::Read(FBitReader& _BitReader)
{
    _BitReader << mFeatureNameID;
    _BitReader << mFeatureIndex;
    _BitReader << mIsAdded;
}
