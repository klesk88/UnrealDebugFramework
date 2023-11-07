//#pragma once
//
////engine
//#include "Containers/Array.h"
//#include "Delegates/IDelegateInstance.h"
//#include "Engine/EngineTypes.h"
//#include "Subsystems/WorldSubsystem.h"
//#include "UObject/ObjectKey.h"
//#include "UObject/WeakObjectPtr.h"
//#include "UObject/WeakObjectPtrTemplates.h"
//
//#include "KLDebugImGuiServerSubsystem_World.generated.h"
//
//class AController;
//class AGameModeBase;
//class APlayerController;
//class UWorld;
//
//UCLASS(Transient)
//class KLDEBUGIMGUISERVER_API UKLDebugImGuiServerSubsystem_World final : public UTickableWorldSubsystem
//{
//    GENERATED_BODY()
//
//public:
//    //UWorldSubsystem
//    bool ShouldCreateSubsystem(UObject* _Outer) const final;
//    bool DoesSupportWorldType(const EWorldType::Type _WorldType) const final;
//    void Initialize(FSubsystemCollectionBase& _Collection);
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
//    UE_NODISCARD static UKLDebugImGuiServerSubsystem_World* TryGet(const UObject& _Object);
//    UE_NODISCARD static UKLDebugImGuiServerSubsystem_World* TryGetMutable(const UObject& _Object);
//
//    void SetShouldTick();
//
//    void OnGameModePostLoginEventHandler(AGameModeBase* _GameMode, APlayerController* _NewPlayer);
//    void OnGameModePostLogoutEventHandle(AGameModeBase* _GameMode, AController* _Exiting);
//
//private:
//    UE_NODISCARD bool IsValid(const UWorld& _World) const;
//
//private:
//    TArray<TWeakObjectPtr<const APlayerController>> mConnectedPlayer;
//    TArray<FObjectKey> mDisconnectedPlayers;
//    FDelegateHandle mOnGameModePostLoginDelegateHandle;
//    FDelegateHandle mOnGameModePostLogoutDelegateHandle;
//    bool mShouldTick = false;
//};
//
//inline ETickableTickType UKLDebugImGuiServerSubsystem_World::GetTickableTickType() const
//{
//    return ETickableTickType::Conditional;
//}
//
//inline bool UKLDebugImGuiServerSubsystem_World::IsTickable() const
//{
//    return mShouldTick;
//}
//
//inline TStatId UKLDebugImGuiServerSubsystem_World::GetStatId() const
//{
//    RETURN_QUICK_DECLARE_CYCLE_STAT(UKLDebugImGuiServerSubsystem_World, STATGROUP_Tickables);
//}
//
//inline void UKLDebugImGuiServerSubsystem_World::SetShouldTick()
//{
//    mShouldTick = true;
//}
//
//inline bool UKLDebugImGuiServerSubsystem_World::IsTickableWhenPaused() const
//{
//    return true;
//}
//
//inline UKLDebugImGuiServerSubsystem_World* UKLDebugImGuiServerSubsystem_World::TryGet(const UObject& _Object)
//{
//    return TryGetMutable(_Object);
//}
