// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

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
class UKLDebugImGuiWorldSubsystem;
class UWorld;

// this is a vase class. Abstract will not make it instantiate
UCLASS(Abstract, Transient)
class KLDEBUGIMGUINETWORKING_API UKLDebugImGuiNetworkingSubsystem_EngineBase : public UEngineSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    // UEngineSubsystem
    void Initialize(FSubsystemCollectionBase& _Collection) override;
    void Deinitialize() override;
    // UEngineSubsystem

    // UTickableWorldSubsystem
    TStatId GetStatId() const override PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::GetStatId, RETURN_QUICK_DECLARE_CYCLE_STAT(UKLDebugImGuiClientSubsystem_World, STATGROUP_Tickables););
    void Tick(float _DeltaTime) override PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::Tick, );
    bool IsTickable() const override;

    ETickableTickType GetTickableTickType() const final;
    bool IsAllowedToTick() const final;
    bool IsTickableWhenPaused() const final;
    bool IsTickableInEditor() const final;
    // UTickableWorldSubsystem

    void SetShouldTick();

protected:
    virtual ETickableTickType GetTickableTickTypeChild() const;

    [[nodiscard]] virtual FKLDebugImGuiNetworkingTCPBase* GetConnectionMutable() PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::GetConnectionMutable, return nullptr;);
    [[nodiscard]] virtual const FKLDebugImGuiNetworkingTCPBase* GetConnection() const PURE_VIRTUAL(UKLDebugImGuiNetworkingSubsystem_EngineBase::GetConnection, return nullptr;);
    [[nodiscard]] virtual bool IsValidWorld(UWorld& _World) const;
    virtual void OnImGuiSusbsytemAdded(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, UWorld& _World);
    virtual void OnImGuiSusbsytemRemoved(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, UWorld& _World);

    [[nodiscard]] bool IsWorldInValidList(const UWorld& _World) const;
    [[nodiscard]] bool HasValidWorlds() const;

    void ClearShouldTick();
    void GatherUpdateData(FKLDebugImGuiNetworkingGameThreadUpdateContextBase& _Context);

private:
    void RegisterDelegates();
    void UnregisterDelegates();

    void OnImGuiWorldSubsystemStateChage(const bool _Added, UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem);

    void OnAddImGuiSubsystem(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem);
    void OnRemoveImGuiSubsystem(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem);

protected:
    bool mShouldTick = false;

private:
    TArray<TWeakObjectPtr<const UWorld>> mCurrentWorlds;
    TArray<TWeakObjectPtr<const UWorld>> mNewWorlds;
    TArray<FObjectKey> mRemovedWorlds;
};

inline ETickableTickType UKLDebugImGuiNetworkingSubsystem_EngineBase::GetTickableTickType() const
{
    // avoid top add this class to the list of tickable objects as well as all CDOs
    return IsTemplate() ? ETickableTickType::Never : GetTickableTickTypeChild();
}

inline ETickableTickType UKLDebugImGuiNetworkingSubsystem_EngineBase::GetTickableTickTypeChild() const
{
    // avoid top add this class to the list of tickable objects
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

inline bool UKLDebugImGuiNetworkingSubsystem_EngineBase::IsWorldInValidList(const UWorld& _World) const
{
    return mCurrentWorlds.Find(&_World) != INDEX_NONE;
}

inline bool UKLDebugImGuiNetworkingSubsystem_EngineBase::HasValidWorlds() const
{
    return !mCurrentWorlds.IsEmpty();
}

inline void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnImGuiSusbsytemAdded(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, UWorld& _World)
{
}

inline void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnImGuiSusbsytemRemoved(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, UWorld& _World)
{
}