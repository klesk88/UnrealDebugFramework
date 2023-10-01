#pragma once

#include "Features/KLDebugImGuiServerObjectContainerFeatures.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Misc/NetworkGuid.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerObjectFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID);
    UE_NODISCARD bool operator==(const FNetworkGUID& _NetworkID) const;

    UE_NODISCARD const FKLDebugImGuiServerObjectContainerFeatures& GetContainer(const EContainerType& _ContainerType) const;
    UE_NODISCARD FKLDebugImGuiServerObjectContainerFeatures& GetContainerMutable(const EContainerType& _ContainerType);

    UE_NODISCARD const TArray<FKLDebugImGuiServerObjectContainerFeatures>& GetContainers() const;
    UE_NODISCARD UObject* GetCachedObjectMutable() const;
    UE_NODISCARD const UObject* GetCachedObject() const;
    UE_NODISCARD const FNetworkGUID& GetNetworkID() const;

private:
    TArray<FKLDebugImGuiServerObjectContainerFeatures> mContainerFeatures;
    FNetworkGUID mNetworkID;
    TWeakObjectPtr<UObject> mCachedObject;
};

inline bool FKLDebugImGuiServerObjectFeatures::operator==(const FNetworkGUID& _NetworkID) const
{
    return mNetworkID == _NetworkID;
}

inline const FKLDebugImGuiServerObjectContainerFeatures& FKLDebugImGuiServerObjectFeatures::GetContainer(const EContainerType& _ContainerType) const
{
    check(_ContainerType != EContainerType::COUNT);
    return mContainerFeatures[static_cast<int32>(_ContainerType)];
}

inline FKLDebugImGuiServerObjectContainerFeatures& FKLDebugImGuiServerObjectFeatures::GetContainerMutable(const EContainerType& _ContainerType)
{
    check(_ContainerType != EContainerType::COUNT);
    return mContainerFeatures[static_cast<int32>(_ContainerType)];
}

inline const TArray<FKLDebugImGuiServerObjectContainerFeatures>& FKLDebugImGuiServerObjectFeatures::GetContainers() const
{
    return mContainerFeatures;
}

inline UObject* FKLDebugImGuiServerObjectFeatures::GetCachedObjectMutable() const
{
    return mCachedObject.Get();
}

inline const UObject* FKLDebugImGuiServerObjectFeatures::GetCachedObject() const
{
    return GetCachedObjectMutable();
}

inline const FNetworkGUID& FKLDebugImGuiServerObjectFeatures::GetNetworkID() const
{
    return mNetworkID;
}