#pragma once

//modules
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"

//engine
#include "Containers/Map.h"
#include "Delegates/IDelegateInstance.h"
#include "Engine/World.h"
#include "Misc/NetworkGuid.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeatureStatusUpdateData;
class UKLDebugImGuiWorldSubsystem;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientData(const UWorld& _World, const FDelegateHandle& _DelegateHandle);
    UE_NODISCARD bool operator==(const FObjectKey& _WorldKey) const;

    void Shutdown(UKLDebugImGuiWorldSubsystem& _ImGuiWorldSubsystem);

    UE_NODISCARD bool OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);

    UE_NODISCARD FNetworkGUID GetLocalPlayerNetworkID() const;
    UE_NODISCARD TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate>& GetFeaturesStatusUpdateMutable();
    UE_NODISCARD const UWorld& GetWorld() const;

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TMap<FObjectKey, FNetworkGUID> mObjectToNetworkID;
    FObjectKey mWorldKey;
    FDelegateHandle mOnFeaturesUpdatedDelegateHandle;
};

inline bool FKLDebugImGuiClientData::operator==(const FObjectKey& _WorldKey) const
{
    return mWorldKey == _WorldKey;
}

inline TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate>& FKLDebugImGuiClientData::GetFeaturesStatusUpdateMutable()
{
    return mPendingFeaturesStatusUpdates;
}

inline const UWorld& FKLDebugImGuiClientData::GetWorld() const
{
    const UWorld* World = Cast<const UWorld>(mWorldKey.ResolveObjectPtr());
    check(World != nullptr);
    return *World;
}