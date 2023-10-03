#include "Features/KLDebugImGuiServerObjectFeatures.h"

//networking runtime module
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"

FKLDebugImGuiServerObjectFeatures::FKLDebugImGuiServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID)
    : mNetworkID(_NetworkID)
{
    ensureMsgf(mNetworkID.IsValid(), TEXT("netowrk id must be valid"));

    mCachedObject = KL::Debug::Networking::Helpers::TryGetObjectFromNetworkGUIDMutable(_World, _NetworkID);
    ensureMsgf(mCachedObject.IsValid(), TEXT("object pointed should be valid"));

    mFeaturesEnable.Reserve(20);
}
