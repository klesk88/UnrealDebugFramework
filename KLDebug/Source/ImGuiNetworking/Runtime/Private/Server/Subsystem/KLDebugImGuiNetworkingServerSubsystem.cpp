#include "Server/Subsystem/KLDebugImGuiNetworkingServerSubsystem.h"

//engine
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

FKLDebugImGuiNetworkingManager_Server UKLDebugImGuiNetworkingServerSubsystem::mServer;

bool UKLDebugImGuiNetworkingServerSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
#if WITH_SERVER_CODE
    const UWorld* World = Cast<const UWorld>(_Outer);
    const bool Result = World && IsValidWorld(*World);
    return Result;
#else
    return false;
#endif
}

void UKLDebugImGuiNetworkingServerSubsystem::Deinitialize()
{
    Super::Deinitialize();

    mServer.Clear(*GetWorld());
}

void UKLDebugImGuiNetworkingServerSubsystem::OnWorldBeginPlay(UWorld& _World)
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

bool UKLDebugImGuiNetworkingServerSubsystem::IsValidWorld(const UWorld& _World) const
{
    return UKismetSystemLibrary::IsServer(&_World) &&
        !UKismetSystemLibrary::IsStandalone(&_World);
}