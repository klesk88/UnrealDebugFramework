// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystem/Engine/KLDebugImGuiNetworkingSubsystem_EngineBase.h"

#include "Subsystem/Engine/KLDebugImGuiNetworkingGameThreadUpdateContextBase.h"
#include "TCP/KLDebugImGuiNetworkingTCPBase.h"

// modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"

void UKLDebugImGuiNetworkingSubsystem_EngineBase::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    mCurrentWorlds.Reserve(10);
    mNewWorlds.Reserve(10);
    mRemovedWorlds.Reserve(10);

    RegisterDelegates();
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::Deinitialize()
{
    Super::Deinitialize();

    UnregisterDelegates();
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::RegisterDelegates()
{
    KL::Debug::ImGui::Framework::OnImGuiWorldSusbsytemStateChange.AddUObject(this, &UKLDebugImGuiNetworkingSubsystem_EngineBase::OnImGuiWorldSubsystemStateChage);
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::UnregisterDelegates()
{
    KL::Debug::ImGui::Framework::OnImGuiWorldSusbsytemStateChange.RemoveAll(this);
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnImGuiWorldSubsystemStateChage(const bool _Added, UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem)
{
    if (_Added)
    {
        OnAddImGuiSubsystem(_ImGuiSubsystem);
    }
    else
    {
        OnRemoveImGuiSubsystem(_ImGuiSubsystem);
    }
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnAddImGuiSubsystem(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem)
{
    check(IsInGameThread());

    UWorld* World = _ImGuiSubsystem.GetWorld();
    if (!World)
    {
        ensureMsgf(false, TEXT("we should alwasy have a valid world"));
        return;
    }

    if (IsValidWorld(*World))
    {
        mCurrentWorlds.Emplace(World);
        mNewWorlds.Emplace(World);
        OnImGuiSusbsytemAdded(_ImGuiSubsystem, *World);
        SetShouldTick();
    }
}

void UKLDebugImGuiNetworkingSubsystem_EngineBase::OnRemoveImGuiSubsystem(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem)
{
    // NOTE: here intentionally we dont check IsValidWorld
    // if for example we travel from a dedicated server to a listen server, that check would fail on a the client subsystem (as now the client is a
    // listen server) so we would never remove the data of the world we are leaving

    check(IsInGameThread());

    UWorld* World = _ImGuiSubsystem.GetWorld();
    if (!World)
    {
        ensureMsgf(false, TEXT("we should alwasy have a valid world"));
        return;
    }

#if !WITH_EDITOR
    // in a package build we can reach here after a first world has been already created.
    if (mCurrentWorlds.IsEmpty())
    {
        return;
    }
#endif

    const int32 Index = mCurrentWorlds.IndexOfByKey(World);
    if (Index != INDEX_NONE)
    {
        mCurrentWorlds.RemoveAt(Index, 1, false);
    }
    else
    {
        return;
    }

    ensureMsgf(mRemovedWorlds.FindByKey(World) == nullptr, TEXT("world already in removal list should not be possible"));
    mRemovedWorlds.Emplace(World);
    OnImGuiSusbsytemRemoved(_ImGuiSubsystem, *World);

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
    _Context.SetNewWorlds(mNewWorlds);
    mNewWorlds.Reset();

    for (const TWeakObjectPtr<const UWorld>& World : mCurrentWorlds)
    {
        // worlds should be always valid just check in case of bugs
        if (LIKELY(World.IsValid()))
        {
            Worlds.Emplace(World.Get());
        }
    }

    _Context.SetRemovedWorlds(mRemovedWorlds);
    mRemovedWorlds.Reset();
}
