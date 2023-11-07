//#pragma once
//
////modules
//#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
//
////engine
//#include "Containers/Array.h"
//#include "Containers/Map.h"
//#include "Engine/EngineTypes.h"
//#include "Misc/NetworkGuid.h"
//#include "Delegates/IDelegateInstance.h"
//#include "Subsystems/WorldSubsystem.h"
//#include "UObject/ObjectKey.h"
//
//#include "KLDebugImGuiClientSubsystem_World.generated.h"
//
//class FKLDebugImGuiFeatureStatusUpdateData;
//class UWorld;
//
//UCLASS(Transient)
//class KLDEBUGIMGUICLIENT_API UKLDebugImGuiClientSubsystem_World final : public UTickableWorldSubsystem
//{
//    GENERATED_BODY()
//
//public:
//    //UWorldSubsystem
//    bool ShouldCreateSubsystem(UObject* _Outer) const final;
//    bool DoesSupportWorldType(const EWorldType::Type _WorldType) const final;
//    void Deinitialize() final;
//    void OnWorldBeginPlay(UWorld& _World) final;
//    //UWorldSubsystem
//
//    //UTickableWorldSubsystem
//    ETickableTickType GetTickableTickType() const final;
//    bool IsTickable() const final;
//    void Tick(float DeltaTime) final;
//    TStatId GetStatId() const final;
//    bool IsTickableWhenPaused() const final;
//    //UTickableWorldSubsystem
//
//    UE_NODISCARD static UKLDebugImGuiClientSubsystem_World* TryGet(const UObject& _Object);
//    UE_NODISCARD static UKLDebugImGuiClientSubsystem_World* TryGetMutable(const UObject& _Object);
//
//    void SetShouldTick();
//    void SetEnstablishConnection();
//
//private:
//    UE_NODISCARD bool IsValid(const UWorld& _World) const;
//
//    void InitWorldDelegates(UWorld& _World);
//    void ClearWorldDelegates(UWorld& _World);
//
//    void OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);
//
//private:
//    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
//    TMap<FObjectKey, FNetworkGUID> mObjectToNetworkID;
//    FDelegateHandle mOnFeaturesUpdatedDelegateHandle;
//    bool mShouldTick = true;
//    bool mHasEnstablishConnection = false;
//};
//
//inline ETickableTickType UKLDebugImGuiClientSubsystem_World::GetTickableTickType() const
//{
//    return ETickableTickType::Conditional;
//}
//
//inline bool UKLDebugImGuiClientSubsystem_World::IsTickable() const
//{
//    return mShouldTick && mHasEnstablishConnection;
//}
//
//inline TStatId UKLDebugImGuiClientSubsystem_World::GetStatId() const
//{
//    RETURN_QUICK_DECLARE_CYCLE_STAT(UKLDebugImGuiClientSubsystem_World, STATGROUP_Tickables);
//}
//
//inline void UKLDebugImGuiClientSubsystem_World::SetShouldTick()
//{
//    mShouldTick = true;
//}
//
//inline bool UKLDebugImGuiClientSubsystem_World::IsTickableWhenPaused() const
//{
//    return true;
//}
//
//inline UKLDebugImGuiClientSubsystem_World* UKLDebugImGuiClientSubsystem_World::TryGet(const UObject& _Object)
//{
//    return TryGetMutable(_Object);
//}
//
//inline void UKLDebugImGuiClientSubsystem_World::SetEnstablishConnection()
//{
//    mHasEnstablishConnection = true;
//}