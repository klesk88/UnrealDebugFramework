// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/AI/KLDebugFeatureAI_Navmesh.h"

#include "Filters/KLDebugImGuiFilterAI.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Canvas/KLDebugImGuiFeatureCanvasInput.h"
#include "ImGui/User/Public/Feature/Interface/Unique/Input/KLDebugImGuiFeatureImGuiInput_Unique.h"
#include "ImGui/User/Public/Feature/Interface/Unique/Input/KLDebugImGuiFeatureRenderInput_Unique.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingReceiveDataInput.h"
#include "ImGui/User/Public/Helpers/KLDebugImGuiHelpers.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshPath.h"
#include "NavMesh/NavMeshRenderingComponent.h"
#include "NavMesh/RecastNavMesh.h"
#include "Serialization/Archive.h"

KL_DEBUG_CREATE_WINDOW(FKLDebugFeatureAI_Navmesh)

void FKLDebugFeatureAI_Navmesh::OnFeatureSelected(const UWorld& _World)
{
}

void FKLDebugFeatureAI_Navmesh::OnFeatureUnselected(const UWorld& _World)
{
}

const FString& FKLDebugFeatureAI_Navmesh::GetWindowName() const
{
    static const FString Name(TEXT("Navmesh"));
    return Name;
}

const FName& FKLDebugFeatureAI_Navmesh::GetImGuiPath() const
{
    static const FName Path(TEXT("AI.Navmesh"));
    return Path;
}

void FKLDebugFeatureAI_Navmesh::DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input)
{
    const UWorld& World = _Input.GetWorld();
    const APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(&World, 0);
    if (!PlayerCamera)
    {
        return;
    }

    FVector Location;
    FRotator CamRot;
    PlayerCamera->GetCameraViewPoint(Location, CamRot);

    const float DistSquare = FVector::DistSquared(mPlayerCameraLocation, Location);
    if (DistSquare > FMath::Square(mMinDistanceForUpdate))
    {
        mPlayerCameraLocation = Location;
    }
}

void FKLDebugFeatureAI_Navmesh::Render(const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const
{
    // IKLDebugImGuiFeatureInterface_Selectable::Render(_Input);
}

bool FKLDebugFeatureAI_Navmesh::ShouldGatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const
{
    return false;
}

void FKLDebugFeatureAI_Navmesh::GatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const
{
}

void FKLDebugFeatureAI_Navmesh::ReceiveData(const FKLDebugImGuiFeature_NetworkingReceiveDataInput& _Input)
{
}

void FKLDebugFeatureAI_Navmesh::CollectNavmeshData(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input)
{
    // based on FGameplayDebuggerCategory_Navmesh::CollectData

    const UWorld& World = _Input.GetWorld();
    const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(&World);
    const ANavigationData* NavData = NavSys ? NavSys->GetDefaultNavDataInstance() : nullptr;
    const ARecastNavMesh* RecastNavMesh = NavData ? Cast<const ARecastNavMesh>(NavData) : nullptr;
    if (!RecastNavMesh)
    {
        return;
    }

    int32 NumTilesPerSide = FMath::Max(static_cast<int32>(mTilesCount), 1);
    NumTilesPerSide += (NumTilesPerSide % 2 == 0) ? 1 : 0;

    const int32 NumTilesToDisplay = NumTilesPerSide * NumTilesPerSide;

    TArray<int32> DeltaX;
    TArray<int32> DeltaY;
    DeltaX.AddUninitialized(NumTilesToDisplay);
    DeltaY.AddUninitialized(NumTilesToDisplay);

    const int32 MinIdx = -(NumTilesPerSide >> 1);
    for (int32 i = 0; i < NumTilesToDisplay; ++i)
    {
        DeltaX[i] = MinIdx + (i % NumTilesPerSide);
        DeltaY[i] = MinIdx + (i / NumTilesPerSide);
    }

    int32 TargetTileX = 0;
    int32 TargetTileY = 0;
    RecastNavMesh->GetNavMeshTileXY(mPlayerCameraLocation, TargetTileX, TargetTileY);

    TArray<int32> TileSet;
    for (int32 Idx = 0; Idx < NumTilesToDisplay; Idx++)
    {
        const int32 NeiX = TargetTileX + DeltaX[Idx];
        const int32 NeiY = TargetTileY + DeltaY[Idx];
        RecastNavMesh->GetNavMeshTilesAt(NeiX, NeiY, TileSet);
    }

    const int32 DetailFlags =
        (1 << static_cast<int32>(ENavMeshDetailFlags::PolyEdges)) |
        (1 << static_cast<int32>(ENavMeshDetailFlags::FilledPolys)) |
        (1 << static_cast<int32>(ENavMeshDetailFlags::NavLinks));

    if (TileSet.Num() == 0)
    {
        mNavmeshRenderData.Reset();
    }
    else
    {
        mNavmeshRenderData.GatherData(RecastNavMesh, DetailFlags, TileSet);
    }
}

void FKLDebugFeatureAI_Navmesh::DrawOnCanvas(FKLDebugImGuiFeatureCanvasInput& _Input) const
{
    _Input.Printf(TEXT("Num dirty areas: {%s}%d"), TEXT("red"), 1);
    _Input.Printf(TEXT("Tile jobs running/remaining: %d / %d"), 12, 23);
    _Input.Printf(FColor::Green, TEXT("Tile jobs running/remaining: %d / %d"), 12, 23);
}
