#include "Subsystem/KLDebugImGuiNetworkingServerSubsystem_World.h"

//engine
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

FKLDebugImGuiNetworkingManager_Server UKLDebugImGuiNetworkingServerSubsystem_World::mServer;

bool UKLDebugImGuiNetworkingServerSubsystem_World::ShouldCreateSubsystem(UObject* _Outer) const
{
#if WITH_SERVER_CODE
    const UWorld* World = Cast<const UWorld>(_Outer);
    const bool Result = World && IsValidWorld(*World);
    return Result;
#else
    return false;
#endif
}

void UKLDebugImGuiNetworkingServerSubsystem_World::Deinitialize()
{
    Super::Deinitialize();

    mServer.Clear(*GetWorld());
}

void UKLDebugImGuiNetworkingServerSubsystem_World::OnWorldBeginPlay(UWorld& _World)
{
    //in editor it can happen we reach here with the client because the world is not initialized.
    if (!IsValidWorld(_World))
    {
        return;
    }

    ensureMsgf(!mServer.IsSocketRegistered(), TEXT("should not be registered"));

    mServer.Init(_World);
    if (!mServer.IsSocketRegistered())
    {
        mServer.Clear(_World);
    }
}

bool UKLDebugImGuiNetworkingServerSubsystem_World::IsValidWorld(const UWorld& _World) const
{
    return UKismetSystemLibrary::IsServer(&_World) &&
        !UKismetSystemLibrary::IsStandalone(&_World);
}