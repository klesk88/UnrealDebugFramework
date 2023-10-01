#include "Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate.h"

//utils
#include "Utils/Public/KLDebugLog.h"

FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate(const FNetworkGUID& _NetworkID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const TArray<uint8>& _Data)
    : mOwnerObjectNetworkID(_NetworkID)
    , mFeatureData(_Data)
    , mFeatureIndex(_FeatureIndex)
{
}

bool FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::CanWrite(const UWorld& _World) const
{
    return true;
}

void FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::WriteChild(const UWorld& _World, FArchive& _BitWriter)
{
    _BitWriter << mOwnerObjectNetworkID;
    _BitWriter << mFeatureIndex;
    _BitWriter << mFeatureData;
}

void FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate::ReadChild(const UWorld& _World, FArchive& _BitReader)
{
    _BitReader << mOwnerObjectNetworkID;
    _BitReader << mFeatureIndex;
    _BitReader << mFeatureData;
}
