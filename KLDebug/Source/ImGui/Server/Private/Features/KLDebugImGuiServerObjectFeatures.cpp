#include "Features/KLDebugImGuiServerObjectFeatures.h"

//networking runtime module
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"

//engine
#include "GameFramework/Actor.h"

FKLDebugImGuiServerObjectFeatures::FKLDebugImGuiServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID)
    : mNetworkID(_NetworkID)
{
    ensureMsgf(mNetworkID.IsValid(), TEXT("netowrk id must be valid"));

    mCachedObject = KL::Debug::Networking::Helpers::TryGetObjectFromNetworkGUIDMutable(_World, _NetworkID);
    ensureMsgf(mCachedObject.IsValid(), TEXT("object pointed should be valid"));

    mFeaturesEnable.Reserve(20);

    if (const AActor* Actor = Cast<const AActor>(mCachedObject.Get()))
    {
        ensureMsgf(Actor->HasAuthority(), TEXT("we should have only objects which we have authority here"));
    }
}
