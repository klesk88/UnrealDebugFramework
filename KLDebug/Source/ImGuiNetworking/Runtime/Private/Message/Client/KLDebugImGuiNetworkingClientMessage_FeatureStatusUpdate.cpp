#include "Message/Client/KLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Engine/World.h"
#include "GameFramework/Actor.h"

FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate(const AActor& _Object)
    : mOwnerObject(&_Object)
{
}

bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::operator==(const AActor& _Object) const
{
    return mOwnerObject.Get() == &_Object;
}

bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::CanWrite(const UWorld& _World) const
{
    return mOwnerObject.IsValid();
}

void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::WriteChild(const UWorld& _World, FBitWriter& _BitWriter)
{
     FNetworkGUID NetworkGUID = TryGetNetworkGuid(*mOwnerObject.Get());
     if (!NetworkGUID.IsValid())
     {
         return;
     }

     _BitWriter << NetworkGUID;
     _BitWriter << mContainerType;

    int32 NumElements = mFeatureData.Num();
    _BitWriter << NumElements;

    for (TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, bool> FeatureData : mFeatureData)
    {
        _BitWriter << FeatureData;
    }
}

void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::ReadChild(const UWorld& _World, FBitReader& _BitReader)
{
    FNetworkGUID NetworkGUID;
    _BitReader << NetworkGUID;

    if (!NetworkGUID.IsValid())
    {
        return;
    }

    mOwnerObject = TryGetObjectFromNetworkGUID(_World, NetworkGUID);

    _BitReader << mContainerType;

    int32 NumElements;
    _BitReader << NumElements;
    for (int32 i = 0; i < NumElements; ++i)
    {
        TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, bool> FeatureData;
        _BitReader << FeatureData;
        mFeatureData.Emplace(FeatureData);
    }
}
