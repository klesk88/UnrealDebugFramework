#include "Subsystem/KLDebugImGuiNetworkingServerSubsystem_World.h"

#include "Subsystem/KLDebugImGuiNetworkingServerSubsystem_Engine.h"

//engine
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

bool UKLDebugImGuiNetworkingServerSubsystem_World::ShouldCreateSubsystem(UObject* _Outer) const
{
    const UKLDebugImGuiNetworkingServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiNetworkingServerSubsystem_Engine::Get();
    if (!ServerEngineSubsystem)
    {
        return false;
    }

    if (const UWorld* World = Cast<const UWorld>(_Outer))
    {
        return IsValid(*World);
    }

    return false;
}

void UKLDebugImGuiNetworkingServerSubsystem_World::Deinitialize()
{
    Super::Deinitialize();

    UKLDebugImGuiNetworkingServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiNetworkingServerSubsystem_Engine::GetMutable();
    if (ServerEngineSubsystem)
    {
        ServerEngineSubsystem->ClearServerFromWorld(*GetWorld());
    }
}

void UKLDebugImGuiNetworkingServerSubsystem_World::OnWorldBeginPlay(UWorld& _World)
{
    //we can reach here because the world was not yet initialized in editor. so double check at this point that we are
    //really on a server
    if (!IsValid(_World))
    {
        return;
    }

    UKLDebugImGuiNetworkingServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiNetworkingServerSubsystem_Engine::GetMutable();
    ServerEngineSubsystem->InitServerFromWorld(_World);
}

bool UKLDebugImGuiNetworkingServerSubsystem_World::IsValid(const UWorld& _World) const
{
    return UKismetSystemLibrary::IsDedicatedServer(&_World);
}
