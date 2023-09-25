#include "Features/KLDebugImGuiNetworking_ServerObjectFeatures.h"

//networking runtime module
#include "ImGuiNetworking/Runtime/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"

FKLDebugImGuiNetworking_ServerObjectFeatures::FKLDebugImGuiNetworking_ServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID)
    : mNetworkID(_NetworkID)
{
    ensureMsgf(mNetworkID.IsValid(), TEXT("netowrk id must be valid"));

    mContainerFeatures.Reserve(static_cast<int32>(EContainerType::COUNT));
    for (int32 i = 0; i < static_cast<int32>(EContainerType::COUNT); ++i)
    {
        const EContainerType ContainerType = static_cast<EContainerType>(i);
        mContainerFeatures.Emplace(ContainerType);
    }

    mCachedObject = KL::Debug::ImGuiNetworking::Helpers::TryGetObjectFromNetworkGUID(_World, _NetworkID);
    ensureMsgf(mCachedObject.IsValid(), TEXT("object pointed should be valid"));
}
