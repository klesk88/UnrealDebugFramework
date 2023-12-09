// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/KLDebugImGuiServerObjectFeatures.h"

// modules
#include "ImGui/Networking/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"

// engine
#include "GameFramework/Actor.h"

FKLDebugImGuiServerObjectFeatures::FKLDebugImGuiServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID)
    : mNetworkID(_NetworkID)
{
    ensureMsgf(mNetworkID.IsValid(), TEXT("netowrk id must be valid"));

    mCachedObject = KL::Debug::ImGuiNetworking::Helpers::TryGetObjectFromNetworkGUIDMutable(_World, _NetworkID);
    ensureMsgf(mCachedObject.IsValid(), TEXT("object pointed should be valid"));

    mFeaturesEnable.Reserve(20);

    if (const AActor* Actor = Cast<const AActor>(mCachedObject.Get()))
    {
        ensureMsgf(Actor->HasAuthority(), TEXT("we should have only objects which we have authority here"));
    }
}
