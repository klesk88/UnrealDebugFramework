#include "Message/Client/KLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Engine/World.h"
#include "GameFramework/Actor.h"

FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate(const FNetworkGUID& _NetworkID)
    : mNetworkID(_NetworkID)
{
}

void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::WriteChild(const UWorld& _World, FBitWriter& _BitWriter)
{
    ensureMsgf(mNetworkID.IsValid(), TEXT("must be valid"));

     _BitWriter << mNetworkID;
     _BitWriter << mContainerType;
     _BitWriter << mFullyRemoved;
     if (mFullyRemoved)
     {
         return;
     }

    int32 NumElements = mFeatureData.Num();
    _BitWriter << NumElements;

    for (TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, bool> FeatureData : mFeatureData)
    {
        _BitWriter << FeatureData;
    }
}

void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::ReadChild(const UWorld& _World, FBitReader& _BitReader)
{
    _BitReader << mNetworkID;
    if (!mNetworkID.IsValid())
    {
        ensureMsgf(false, TEXT("must be valid"));
        return;
    }

    _BitReader << mContainerType;
    _BitReader << mFullyRemoved;
    if (mFullyRemoved)
    {
        return;
    }

    int32 NumElements;
    _BitReader << NumElements;
    for (int32 i = 0; i < NumElements; ++i)
    {
        TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, bool> FeatureData;
        _BitReader << FeatureData;
        mFeatureData.Emplace(FeatureData);
    }
}
