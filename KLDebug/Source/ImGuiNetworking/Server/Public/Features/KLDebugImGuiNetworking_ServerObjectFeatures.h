#pragma once

#include "Features/KLDebugImGuiNetworking_ServerObjectContainerFeatures.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Misc/NetworkGuid.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class UWorld;

class KLDEBUGIMGUINETWORKINGSERVER_API FKLDebugImGuiNetworking_ServerObjectFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworking_ServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID);
    UE_NODISCARD bool operator==(const FNetworkGUID& _NetworkID) const;

    const FKLDebugImGuiNetworking_ServerObjectContainerFeatures& GetContainer(const EContainerType& _ContainerType) const;
    FKLDebugImGuiNetworking_ServerObjectContainerFeatures& GetContainerMutable(const EContainerType& _ContainerType);

private:
    TArray<FKLDebugImGuiNetworking_ServerObjectContainerFeatures> mContainerFeatures;
    FNetworkGUID mNetworkID;
    TWeakObjectPtr<const UObject> mCachedObject;
};

inline bool FKLDebugImGuiNetworking_ServerObjectFeatures::operator==(const FNetworkGUID& _NetworkID) const
{
    return mNetworkID == _NetworkID;
}

inline const FKLDebugImGuiNetworking_ServerObjectContainerFeatures& FKLDebugImGuiNetworking_ServerObjectFeatures::GetContainer(const EContainerType& _ContainerType) const
{
    check(_ContainerType != EContainerType::COUNT);
    return mContainerFeatures[static_cast<int32>(_ContainerType)];
}

inline FKLDebugImGuiNetworking_ServerObjectContainerFeatures& FKLDebugImGuiNetworking_ServerObjectFeatures::GetContainerMutable(const EContainerType& _ContainerType)
{
    check(_ContainerType != EContainerType::COUNT);
    return mContainerFeatures[static_cast<int32>(_ContainerType)];
}