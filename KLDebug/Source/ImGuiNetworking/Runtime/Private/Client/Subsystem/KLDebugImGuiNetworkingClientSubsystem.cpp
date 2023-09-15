#include "Client/Subsystem/KLDebugImGuiNetworkingClientSubsystem.h"

//engine
#include "Engine/World.h"

bool UKLDebugImGuiNetworkingClientSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    const UWorld* World = Cast<const UWorld>(_Outer);
    const bool Result = World && IsValidWorld(*World);
    return Result;
}

void UKLDebugImGuiNetworkingClientSubsystem::Deinitialize()
{
    Super::Deinitialize();

    mClient.Clear(*GetWorld());
}

void UKLDebugImGuiNetworkingClientSubsystem::OnWorldBeginPlay(UWorld& _World)
{
    //in editor it can happen we reach here with the client because the world is not initialized.
    if (!IsValidWorld(_World))
    {
        return;
    }

    ensureMsgf(!mClient.IsSocketRegistered(), TEXT("should not be registered"));

    mClient.Init(_World);
    if (!mClient.IsSocketRegistered())
    {
        mClient.Clear(_World);
    }
}

bool UKLDebugImGuiNetworkingClientSubsystem::IsValidWorld(const UWorld& _World) const
{
    return _World.GetNetMode() == NM_Client;
}
