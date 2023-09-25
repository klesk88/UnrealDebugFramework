#include "Subsystem/KLDebugImGuiNetworkingClientSubsystem_World.h"

//engine
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"

bool UKLDebugImGuiNetworkingClientSubsystem_World::ShouldCreateSubsystem(UObject* _Outer) const
{
    if (const UWorld* World = Cast<const UWorld>(_Outer))
    {
        return IsValid(*World);
    }

    return false;
}

void UKLDebugImGuiNetworkingClientSubsystem_World::Deinitialize()
{
    Super::Deinitialize();

    mClient.ClearFromWorld(*GetWorld());
}

void UKLDebugImGuiNetworkingClientSubsystem_World::OnWorldBeginPlay(UWorld& _World)
{
    //we can reach here because the world was not yet initialized in editor. so double check at this point that we are
    //really on a client
    if (!IsValid(_World))
    {
        return;
    }

    ensureMsgf(!mClient.IsSocketRegistered(), TEXT("should not be registered"));

    mClient.InitFromWorld(_World);
    if (!mClient.IsSocketRegistered())
    {
        mClient.ClearFromWorld(_World);
    }
}

bool UKLDebugImGuiNetworkingClientSubsystem_World::IsValid(const UWorld& _World) const
{
    const ENetMode NetMode = _World.GetNetMode();
    return NetMode == NM_Client;
}
