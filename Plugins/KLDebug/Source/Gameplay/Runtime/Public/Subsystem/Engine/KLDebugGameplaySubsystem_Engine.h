// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Camera/Manager/KLDebugGameplayCameraManager.h"

// engine
#include "Subsystems/EngineSubsystem.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "KLDebugGameplaySubsystem_Engine.generated.h"

class ULocalPlayer;

UCLASS(Transient)
class KLDEBUGGAMEPLAYRUNTIME_API UKLDebugGameplaySubsystem_Engine final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    [[nodiscard]] static UKLDebugGameplaySubsystem_Engine* TryGetMutable();
    [[nodiscard]] static const UKLDebugGameplaySubsystem_Engine* TryGet();

    void ToogleDebugCamera(const TWeakObjectPtr<const ULocalPlayer> _LocalPlayer);
    void TooglePause(const TWeakObjectPtr<const ULocalPlayer> _LocalPlayer);

private:
    FKLDebugGameplayCameraManager mCameraManager;
};

inline const UKLDebugGameplaySubsystem_Engine* UKLDebugGameplaySubsystem_Engine::TryGet()
{
    return TryGetMutable();
}

inline void UKLDebugGameplaySubsystem_Engine::ToogleDebugCamera(const TWeakObjectPtr<const ULocalPlayer> _LocalPlayer)
{
    mCameraManager.ToogleDebugCamera(_LocalPlayer);
}
