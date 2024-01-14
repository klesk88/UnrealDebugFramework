// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "BottomBar/KLDebugExampleBottomBar.h"

// modules
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "User/Framework/Public/Window/BottomBar/KLDebugModeInterfaceAllInput.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "Engine/DebugCameraController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

KL_DEBUG_CREATE_BOTTOMBAR(FKLDebugExampleBottomBar)

const FString& FKLDebugExampleBottomBar::GetFriendlyName() const
{
    static const FString Name(TEXT("ExampleBar"));
    return Name;
}

void FKLDebugExampleBottomBar::Draw(const FKLDebugBottomBarDrawInput& _Input) const
{
    const UWorld& World = _Input.GetWorld();

    ImGui::Text("Frame: [%llu]", UKismetSystemLibrary::GetFrameCount());
    ImGui::SameLine();
    ImGui::Text(" Time: [%.3f]", World.GetTimeSeconds());

    const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(&World, 0);
    const ACharacter* PlayerCharacter = PlayerController ? PlayerController->GetCharacter() : nullptr;
    if (!PlayerCharacter)
    {
        const ADebugCameraController* DebugCamera = Cast<const ADebugCameraController>(PlayerController);
        if (DebugCamera && DebugCamera->OriginalControllerRef)
        {
            PlayerCharacter = DebugCamera->OriginalControllerRef->GetCharacter();
        }
    }

    if (PlayerCharacter)
    {
        ImGui::SameLine();
        ImGui::Text(" Player: [%ls]", *PlayerCharacter->GetActorLocation().ToString());
    }

    const APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(&World, 0);
    if (PlayerCamera)
    {
        FVector CameraLocation;
        FRotator CamRot;
        PlayerCamera->GetCameraViewPoint(CameraLocation, CamRot);

        ImGui::SameLine();
        ImGui::Text(" Camera: [%ls]", *CameraLocation.ToString());
    }

    ImGui::SameLine();
    ImGui::Text(" FPS: [%.2f]", World.GetDeltaSeconds() != 0.f ? 1.f / World.GetDeltaSeconds() : 0.f);
}
