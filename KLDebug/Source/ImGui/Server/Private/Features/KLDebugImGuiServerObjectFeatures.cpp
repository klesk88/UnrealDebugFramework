#include "Features/KLDebugImGuiServerObjectFeatures.h"

//networking runtime module
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"

FKLDebugImGuiServerObjectFeatures::FKLDebugImGuiServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID)
    : mNetworkID(_NetworkID)
{
    ensureMsgf(mNetworkID.IsValid(), TEXT("netowrk id must be valid"));

    mContainerFeatures.Reserve(static_cast<int32>(EImGuiInterfaceType::COUNT));
    for (int32 i = 0; i < static_cast<int32>(EImGuiInterfaceType::COUNT); ++i)
    {
        const EImGuiInterfaceType ContainerType = static_cast<EImGuiInterfaceType>(i);
        mContainerFeatures.Emplace(ContainerType);
    }

    mCachedObject = KL::Debug::Networking::Helpers::TryGetObjectFromNetworkGUIDMutable(_World, _NetworkID);
    ensureMsgf(mCachedObject.IsValid(), TEXT("object pointed should be valid"));
}
