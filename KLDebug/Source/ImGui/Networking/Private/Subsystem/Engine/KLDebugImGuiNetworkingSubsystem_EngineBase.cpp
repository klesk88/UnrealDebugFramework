#include "Subsystem/Engine/KLDebugImGuiNetworkingSubsystem_EngineBase.h"

#include "Subsystem/Engine/KLDebugImGuiNetworkingGameThreadUpdateContextBase.h"
#include "TCP/KLDebugImGuiNetworkingTCPBase.h"

void UKLDebugImGuiNetworkingSubsystem_EngineBase::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    mCurrentWorlds.Reserve(10);
    mRemovedWorlds.Reserve(10);

    RegisterWorldDelegates();
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::Deinitialize()
{
    Super::Deinitialize();

    UnregisterWorldDelegates();
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::RegisterWorldDelegates()
{
    FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UKLDebugImGuiNetworkingSubsystem_EngineBase::OnWorldInitialze);
    FWorldDelegates::OnWorldCleanup.AddUObject(this, &UKLDebugImGuiNetworkingSubsystem_EngineBase::OnWorldRemoved);
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::UnregisterWorldDelegates()
{
    FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
    FWorldDelegates::OnWorldCleanup.RemoveAll(this);
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnWorldInitialze(UWorld* _World, const UWorld::InitializationValues _IVS)
{
    check(IsInGameThread());

    if (_World && IsValidWorld(*_World))
    {
        mCurrentWorlds.Emplace(_World);
        OnWorldAdded(*_World);
    }
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnWorldRemoved(UWorld* _World, bool _SessionEnded, bool _CleanupResources)
{
    check(IsInGameThread());

    if (!_World)
    {
        return;
    }

#if !WITH_EDITOR
    //in a package build we can reach here after a first world has been already created.
    if (mCurrentWorlds.IsEmpty())
    {
        return;
    }
#endif

    const int32 Index = mCurrentWorlds.IndexOfByKey(_World);
    if (Index != INDEX_NONE)
    {
        mCurrentWorlds.RemoveAt(Index, 1, false);
    }
    else
    {
        return;
    }

    ensureMsgf(mRemovedWorlds.FindByKey(_World) == nullptr, TEXT("world already in removal list should not be possible"));
    mRemovedWorlds.Emplace(_World);
    OnWorldRemoved(*_World);

    SetShouldTick();
}

bool UKLDebugImGuiNetworkingSubsystem_EngineBase::IsValidWorld(UWorld& _World) const
{
    switch (_World.WorldType)
    {
    case EWorldType::Type::Game:
    case EWorldType::Type::PIE:
    case EWorldType::Type::GameRPC:
        return true;
    case EWorldType::Type::Editor:
    case EWorldType::Type::GamePreview:
    case EWorldType::Type::EditorPreview:
    case EWorldType::Type::None:
    case EWorldType::Type::Inactive:
        return false;
    default:
        ensureMsgf(false, TEXT("case not handled"));
        return false;
    }
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::GatherUpdateData(FKLDebugImGuiNetworkingGameThreadUpdateContextBase& _Context)
{
    TArray<const UWorld*>& Worlds = _Context.GetWorldsMutable();
    Worlds.Reserve(mCurrentWorlds.Num());

    for (const TWeakObjectPtr<const UWorld>& World : mCurrentWorlds)
    {
        //worlds should be always valid just check in case of bugs
        if (LIKELY(World.IsValid()))
        {
            Worlds.Emplace(World.Get());
        }
    }

    _Context.SetRemovedWorlds(mRemovedWorlds);
    mRemovedWorlds.Reset();
}
