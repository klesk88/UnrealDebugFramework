// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/AI/KLDebugFeatureAI_Navmesh.h"

#include "Filters/KLDebugImGuiFilterAI.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureUniqueAllInputs.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingReceiveDataInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingTickInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugNetworkingFeature_RequestUpdateInput.h"
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

TUniquePtr<FDebugRenderSceneProxy> FKLDebugFeatureAI_Navmesh::CreateDebugSceneProxy(FKLDebugFeatureSceneProxyInput_Unique& _Input)
{
    TUniquePtr<FNavMeshSceneProxy> NavMeshSceneProxy = MakeUnique<FNavMeshSceneProxy>(&_Input.GetRenderingComponent(), &mNavmeshRenderData, true);

#if WITH_RECAST && !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    TUniquePtr<FNavMeshDebugDrawDelegateHelper> OutDelegateHelper2 = MakeUnique<FNavMeshDebugDrawDelegateHelper>(FNavMeshDebugDrawDelegateHelper());
    OutDelegateHelper2->InitDelegateHelper(NavMeshSceneProxy.Get());
    _Input.SetDrawDelegateHelper(MoveTemp(OutDelegateHelper2));
#endif    // WITH_RECAST && !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

    return NavMeshSceneProxy;
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

void FKLDebugFeatureAI_Navmesh::Server_FeatureUpdate(const FKLDebugNetworkingFeature_RequestUpdateInput& _Input) const
{
    FNavMeshSceneProxyData ProxyData;
    FVector Location = FVector::ZeroVector;

    _Input.GetReader() << Location;

    CollectNavmeshData(_Input.GetWorld(), Location, ProxyData);
    ProxyData.Serialize(_Input.GetWriter());
}

void FKLDebugFeatureAI_Navmesh::Client_Tick(FKLDebugImGuiFeature_NetworkingTickInput& _Input)
{
    if (mRenderData)
    {
        _Input.SetUpdateSceneProxy();
        mRenderData = false;
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

    const float DistSquare = FVector::DistSquared(mPlayerCameraLocation, Location);
    if (DistSquare > FMath::Square(mMinDistanceForUpdate))
    {
        mPlayerCameraLocation = Location;
        FArchive& Writer = _Input.GetWriter();
        Writer << mPlayerCameraLocation;
    }
}

void FKLDebugFeatureAI_Navmesh::Client_ReceiveData(const FKLDebugImGuiFeature_NetworkingReceiveDataInput& _Input)
{
    mNavmeshRenderData.Serialize(_Input.GetArchiveMutable());
    mRenderData = true;
}

void FKLDebugFeatureAI_Navmesh::Tick(FKLDebugFeatureTickInput_Unique& _Input)
{
    // const UWorld& World = _Input.GetWorld();
    // const APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(&World, 0);
    // if (!PlayerCamera)
    //{
    //     return;
    // }

    // FVector Location = FVector::ZeroVector;
    // FRotator CamRot = FRotator::ZeroRotator;
    // PlayerCamera->GetCameraViewPoint(Location, CamRot);

    // const float DistSquare = FVector::DistSquared(mPlayerCameraLocation, Location);
    // if (DistSquare > FMath::Square(mMinDistanceForUpdate))
    //{
    //     mPlayerCameraLocation = Location;
    //     CollectNavmeshData(World, mPlayerCameraLocation, mNavmeshRenderData);

    //    _Input.GetUpdateFlagsMutable().SetFag(FKLDebugImGuiFeatureInputFlags::EFeatureUpdateFlags::SceneProxy);
    //}
}

void FKLDebugFeatureAI_Navmesh::CollectNavmeshData(const UWorld& _World, const FVector& _Location, FNavMeshSceneProxyData& _ProxyData) const
{
    // based on FGameplayDebuggerCategory_Navmesh::CollectData

    const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(&_World);
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
    RecastNavMesh->GetNavMeshTileXY(_Location, TargetTileX, TargetTileY);

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
        _ProxyData.Reset();
    }
    else
    {
        _ProxyData.GatherData(RecastNavMesh, DetailFlags, TileSet);
    }
}

void FKLDebugFeatureAI_Navmesh::DrawOnCanvas(FKLDebugFeatureDrawCanvasInput_Unique& _Input) const
{
    _Input.Printf(TEXT("Num dirty areas: {%s}%d"), TEXT("red"), 1);
    _Input.Printf(TEXT("Tile jobs running/remaining: %d / %d"), 12, 23);
    _Input.Printf(FColor::Green, TEXT("Tile jobs running/remaining: %d / %d"), 12, 23);
}
