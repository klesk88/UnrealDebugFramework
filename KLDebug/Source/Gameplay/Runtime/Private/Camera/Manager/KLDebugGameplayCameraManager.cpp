// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Camera/Manager/KLDebugGameplayCameraManager.h"

#include "Camera/KLDebugGameplayCameraController.h"

// engine
#include "Engine/DebugCameraController.h"
#include "Engine/LocalPlayer.h"
#include "Engine/RendererSettings.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

FKLDebugGameplayCameraManager::FKLDebugGameplayCameraManager()
    : mCameraClass(AKLDebugGameplayCameraController::StaticClass())
{
}

void FKLDebugGameplayCameraManager::UpdateCameraClass(const TSubclassOf<AKLDebugGameplayCameraController>& _NewCameraClass)
{
    if (_NewCameraClass.Get())
    {
        mCameraClass = _NewCameraClass;
    }
}

void FKLDebugGameplayCameraManager::ToogleDebugCamera(const TWeakObjectPtr<const ULocalPlayer>& _LocalPlayer)
{
    if (mDebugCameraIsEnable)
    {
        DisableDebugCamera(_LocalPlayer);
    }
    else
    {
        EnableDebugCamera(_LocalPlayer);
    }
}

void FKLDebugGameplayCameraManager::EnableDebugCamera(const TWeakObjectPtr<const ULocalPlayer>& _LocalPlayer)
{
    if (!_LocalPlayer.IsValid() || !mCameraClass.Get())
    {
        return;
    }

    URendererSettings* RenderSettings = GetMutableDefault<URendererSettings>();
    if (RenderSettings)
    {
        RenderSettings->bDefaultFeatureMotionBlur = false;
    }

    UWorld* World = _LocalPlayer->GetWorld();
    APlayerController* PlayerController = _LocalPlayer->GetPlayerController(World);
    mDebugCameraIsEnable = true;

    // spawn if necessary
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = PlayerController->GetInstigator();
    ADebugCameraController* DebugCameraController = World->SpawnActor<ADebugCameraController>(mCameraClass.Get(), SpawnInfo);
    if (DebugCameraController)
    {
        // set up new controller
        DebugCameraController->OnActivate(PlayerController);

        // then switch to it
        PlayerController->Player->SwitchController(DebugCameraController);
        // DebugCameraController->SetPawn(PlayerController->GetPawn());
    }
}

void FKLDebugGameplayCameraManager::DisableDebugCamera(const TWeakObjectPtr<const ULocalPlayer>& _LocalPlayer)
{
    if (!_LocalPlayer.IsValid())
    {
        return;
    }

    mDebugCameraIsEnable = false;
    UWorld* World = _LocalPlayer->GetWorld();
    APlayerController* PlayerController = _LocalPlayer->GetPlayerController(World);
    ADebugCameraController* DebugCameraController = Cast<ADebugCameraController>(PlayerController);
    if (DebugCameraController && DebugCameraController->OriginalPlayer && DebugCameraController->OriginalControllerRef)
    {
        DebugCameraController->OriginalPlayer->SwitchController(DebugCameraController->OriginalControllerRef);
        DebugCameraController->OnDeactivate(DebugCameraController->OriginalControllerRef);
    }
}
