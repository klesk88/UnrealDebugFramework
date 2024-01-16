// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "BottomBar/KLDebugExampleBottomBar.h"

// modules
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "User/Framework/Public/Window/BottomBar/KLDebugModeInterfaceAllInput.h"
#include "User/Networking/Public/Window/KLDebugNetworkingWindowAllInputs.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "Engine/DebugCameraController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshPath.h"
#include "NavMesh/NavMeshRenderingComponent.h"
#include "NavMesh/RecastNavMesh.h"

KL_DEBUG_CREATE_BOTTOMBAR(FKLDebugExampleBottomBar)

const FString& FKLDebugExampleBottomBar::GetFriendlyName() const
{
    static const FString Name(TEXT("ExampleBar"));
    return Name;
}

TUniquePtr<IKLDebugContextInterface> FKLDebugExampleBottomBar::GetContext(const FKLDebugContextGetterInput& _Input) const
{
    if (_Input.GetCurrentNetMode() == ENetMode::NM_Client)
    {
        return MakeUnique<FDebugExampleBottombarClientContext>();
    }
    else if (_Input.GetCurrentNetMode() != ENetMode::NM_Standalone)
    {
        return MakeUnique<FDebugExampleBottombarServerContext>();
    }

    return nullptr;
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

    if (_Input.GetWorld().GetNetMode() == ENetMode::NM_Client)
    {
        const FDebugExampleBottombarClientContext& Context = _Input.GetContext<FDebugExampleBottombarClientContext>();

        ImGui::Text(" Network Nav Tile X: [%d]", Context.mSyncedCurrentTileX);
        ImGui::Text(" Network Nav Tile Y: [%d]", Context.mSyncedCurrentTileY);
        ImGui::Text(" Network Nav Tile Count: [%d]", Context.mRequestServerTileCount);
    }
}

void FKLDebugExampleBottomBar::Server_Tick(const FKLDebugNetworkingWindowServerTickInput& _Input)
{
    const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(&_Input.GetWorld());
    const ANavigationData* NavData = NavSys ? NavSys->GetDefaultNavDataInstance() : nullptr;
    const ARecastNavMesh* RecastNavMesh = NavData ? Cast<const ARecastNavMesh>(NavData) : nullptr;
    if (!RecastNavMesh)
    {
        return;
    }

    const UWorld& World = _Input.GetWorld();
    const APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(&World, 0);
    if (!PlayerCamera)
    {
        return;
    }

    FVector Location = FVector::ZeroVector;
    FRotator CamRot = FRotator::ZeroRotator;
    PlayerCamera->GetCameraViewPoint(Location, CamRot);

    int32 TargetTileX = 0;
    int32 TargetTileY = 0;
    RecastNavMesh->GetNavMeshTileXY(Location, TargetTileX, TargetTileY);

    FDebugExampleBottombarServerContext& Context = _Input.GetContextMutable<FDebugExampleBottombarServerContext>();
    if (Context.mSyncedCurrentTileX == TargetTileX && Context.mSyncedCurrentTileY == TargetTileY)
    {
        return;
    }

    Context.mSyncedCurrentTileX = TargetTileX;
    Context.mSyncedCurrentTileY = TargetTileY;

    FArchive& Archive = _Input.GetArchiveMutable();
    Archive << TargetTileY;
    Archive << TargetTileX;
}

void FKLDebugExampleBottomBar::Server_FeatureUpdate(const FKLDebugNetworkingWindowRequestUpdateInput& _Input) const
{
    const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(&_Input.GetWorld());
    const ANavigationData* NavData = NavSys ? NavSys->GetDefaultNavDataInstance() : nullptr;
    const ARecastNavMesh* RecastNavMesh = NavData ? Cast<const ARecastNavMesh>(NavData) : nullptr;
    if (!RecastNavMesh)
    {
        return;
    }

    int32 TilesNum = RecastNavMesh->GetNavMeshTilesCount();
    _Input.GetWriter() << TilesNum;
}

void FKLDebugExampleBottomBar::Client_ReceiveData(const FKLDebugNetworkingWindowReceiveDataInput& _Input)
{
    FDebugExampleBottombarClientContext& Context = _Input.GetContextMutable<FDebugExampleBottombarClientContext>();
    switch (_Input.GetReceiveMessageType())
    {
    case EKLDebugNetworkReceiveMessageType::RequestUpdate:
        _Input.GetArchiveMutable() << Context.mRequestServerTileCount;
        break;
    case EKLDebugNetworkReceiveMessageType::ServerTick:
        _Input.GetArchiveMutable() << Context.mSyncedCurrentTileX;
        _Input.GetArchiveMutable() << Context.mSyncedCurrentTileY;
        break;
    }
}

void FKLDebugExampleBottomBar::Client_Tick(FKLDebugNetworkingWindowClientTickInput& _Input)
{
    bool Update = false;
    _Input.GetArchiveMutable() << Update;
}
