#include "Subsystem/Engine/KLDebugGameplaySubsystem_Engine.h"

//engine
#include "Engine/Engine.h"

UKLDebugGameplaySubsystem_Engine* UKLDebugGameplaySubsystem_Engine::TryGetMutable()
{
    return GEngine->GetEngineSubsystem<UKLDebugGameplaySubsystem_Engine>();
}
