#include "Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Engine/World.h"
#include "GameFramework/Actor.h"

FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate(const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _ContainerType)
    : mNetworkID(_NetworkID)
    , mContainerType(_ContainerType)
{
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::WriteChild(const UWorld& _World, FArchive& _BitWriter)
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

    for (FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData& FeatureData : mFeatureData)
    {
        FeatureData.Write(_BitWriter);
    }
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::ReadChild(const UWorld& _World, FArchive& _BitReader)
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
    mFeatureData.Reserve(NumElements);
    for (int32 i = 0; i < NumElements; ++i)
    {
        FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData& NewData = mFeatureData.Emplace_GetRef();
        NewData.Read(_BitReader);
    }
}
