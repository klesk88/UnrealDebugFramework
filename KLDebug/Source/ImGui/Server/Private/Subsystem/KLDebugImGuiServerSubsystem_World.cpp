#include "Subsystem/KLDebugImGuiServerSubsystem_World.h"

#include "Subsystem/KLDebugImGuiServerSubsystem_Engine.h"

//engine
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

bool UKLDebugImGuiServerSubsystem_World::ShouldCreateSubsystem(UObject* _Outer) const
{
    const UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::Get();
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

void UKLDebugImGuiServerSubsystem_World::Deinitialize()
{
    Super::Deinitialize();

    UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::GetMutable();
    if (ServerEngineSubsystem)
    {
        ServerEngineSubsystem->ClearServerFromWorld(*GetWorld());
    }
}

void UKLDebugImGuiServerSubsystem_World::OnWorldBeginPlay(UWorld& _World)
{
    //we can reach here because the world was not yet initialized in editor. so double check at this point that we are
    //really on a server
    if (!IsValid(_World))
    {
        return;
    }

    UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::GetMutable();
    ServerEngineSubsystem->InitServerFromWorld(_World);
}

bool UKLDebugImGuiServerSubsystem_World::IsValid(const UWorld& _World) const
{
    return UKismetSystemLibrary::IsDedicatedServer(&_World);
}
