#include "Subsystem/KLDebugImGuiNetworkingServerSubsystem_World.h"

//engine
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
#include "Subsystem/KLDebugImGuiNetworkingServerSubsystem_Engine.h"
#endif

FKLDebugImGuiNetworkingManager_Server UKLDebugImGuiNetworkingServerSubsystem_World::mServer;

bool UKLDebugImGuiNetworkingServerSubsystem_World::ShouldCreateSubsystem(UObject* _Outer) const
{
    if (const UWorld* World = Cast<const UWorld>(_Outer))
    {
        return IsValid(*World);
    }

    return false;
}

void UKLDebugImGuiNetworkingServerSubsystem_World::Deinitialize()
{
    Super::Deinitialize();

    mServer.Clear(*GetWorld());
}

void UKLDebugImGuiNetworkingServerSubsystem_World::OnWorldBeginPlay(UWorld& _World)
{
    //we can reach here because the world was not yet initialized in editor. so double check at this point that we are
    //really on a server
    if (!IsValid(_World))
    {
        return;
    }

    ensureMsgf(!mServer.IsSocketRegistered(), TEXT("should not be registered"));

    mServer.Init(_World);
    if (!mServer.IsSocketRegistered())
    {
        mServer.Clear(_World);
    }

#if !WITH_EDITOR
    UKLDebugImGuiNetworkingServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiNetworkingServerSubsystem_Engine::GetMutable();
    if (ServerEngineSubsystem)
    {
        ServerEngineSubsystem->CookedOnly_InitFeatureMapIfNeeded();
    }
#endif
}

bool UKLDebugImGuiNetworkingServerSubsystem_World::IsValid(const UWorld& _World) const
{
    return UKismetSystemLibrary::IsDedicatedServer(&_World);
}
