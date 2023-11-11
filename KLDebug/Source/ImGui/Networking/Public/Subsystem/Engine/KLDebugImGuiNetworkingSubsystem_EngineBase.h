#pragma once

// engine
#include "Containers/Array.h"
#include "Engine/World.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "KLDebugImGuiNetworkingSubsystem_EngineBase.generated.h"

class FKLDebugImGuiNetworkingGameThreadUpdateContextBase;
class FKLDebugImGuiNetworkingTCPBase;
class IKLDebugImGuiNetworkingWorldProviderInterface;
class UWorld;

//this is a vase class. Abstract will not make it instantiate
UCLASS(Abstract, Transient)
class KLDEBUGIMGUINETWORKING_API UKLDebugImGuiNetworkingSubsystem_EngineBase : public UEngineSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    //UEngineSubsystem
    void Initialize(FSubsystemCollectionBase& _Collection) override;
    void Deinitialize() override;
    //UEngineSubsystem

    //UTickableWorldSubsystem
    TStatId GetStatId() const override PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::GetStatId, RETURN_QUICK_DECLARE_CYCLE_STAT(UKLDebugImGuiClientSubsystem_World, STATGROUP_Tickables););
    void Tick(float _DeltaTime) override PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::Tick, );
    bool IsTickable() const override;

    ETickableTickType GetTickableTickType() const final;
    bool IsAllowedToTick() const final;
    bool IsTickableWhenPaused() const final;
    bool IsTickableInEditor() const final;
    //UTickableWorldSubsystem

    void SetShouldTick();

protected:
    virtual ETickableTickType GetTickableTickTypeChild() const;

    UE_NODISCARD virtual FKLDebugImGuiNetworkingTCPBase* GetConnectionMutable() PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::GetConnectionMutable, return nullptr;);
    UE_NODISCARD virtual const FKLDebugImGuiNetworkingTCPBase* GetConnection() const PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::GetConnection, return nullptr;);
    UE_NODISCARD virtual bool IsValidWorld(UWorld& _World) const;
    virtual void OnWorldAdded(UWorld& _World);
    virtual void OnWorldRemoved(UWorld& _World);

    void ClearShouldTick();
    void GatherUpdateData(FKLDebugImGuiNetworkingGameThreadUpdateContextBase& _Context);

private:
    void RegisterWorldDelegates();
    void UnregisterWorldDelegates();

    void OnWorldInitialze(UWorld* _World, const UWorld::InitializationValues _IVS);
    void OnWorldRemoved(UWorld* _World, bool _SessionEnded, bool _CleanupResources);

protected:
    bool mShouldTick = false;

private:
    TArray<TWeakObjectPtr<const UWorld>> mCurrentWorlds;
    TArray<FObjectKey> mRemovedWorlds;
};

inline ETickableTickType UKLDebugImGuiNetworkingSubsystem_EngineBase::GetTickableTickType() const
{
    //avoid top add this class to the list of tickable objects as well as all CDOs
    return IsTemplate() ? ETickableTickType::Never : GetTickableTickTypeChild();
}

inline ETickableTickType UKLDebugImGuiNetworkingSubsystem_EngineBase::GetTickableTickTypeChild() const
{
    //avoid top add this class to the list of tickable objects
    return ETickableTickType::Never;
}

inline bool UKLDebugImGuiNetworkingSubsystem_EngineBase::IsAllowedToTick() const
{
    return !IsTemplate();
}

inline bool UKLDebugImGuiNetworkingSubsystem_EngineBase::IsTickable() const
{
    return mShouldTick;
}

inline bool UKLDebugImGuiNetworkingSubsystem_EngineBase::IsTickableWhenPaused() const
{
    return true;
}

inline bool UKLDebugImGuiNetworkingSubsystem_EngineBase::IsTickableInEditor() const
{
    return true;
}

inline void UKLDebugImGuiNetworkingSubsystem_EngineBase::SetShouldTick()
{
    mShouldTick = true;
}

inline void UKLDebugImGuiNetworkingSubsystem_EngineBase::ClearShouldTick()
{
    mShouldTick = false;
}

inline void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnWorldAdded(UWorld& _World)
{
}

inline void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnWorldRemoved(UWorld& _World)
{
}