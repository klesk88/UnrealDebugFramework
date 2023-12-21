// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystem/Engine/KLDebugGameplaySubsystem_Engine.h"

// engine
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

UKLDebugGameplaySubsystem_Engine* UKLDebugGameplaySubsystem_Engine::TryGetMutable()
{
    return GEngine->GetEngineSubsystem<UKLDebugGameplaySubsystem_Engine>();
}

void UKLDebugGameplaySubsystem_Engine::TooglePause(const TWeakObjectPtr<const ULocalPlayer> _LocalPlayer)
{
    const UWorld* World = _LocalPlayer.IsValid() ? _LocalPlayer->GetWorld() : nullptr;
    if (World)
    {
        mPauseState = !mPauseState;
        UGameplayStatics::SetGamePaused(World, mPauseState);
    }
}
