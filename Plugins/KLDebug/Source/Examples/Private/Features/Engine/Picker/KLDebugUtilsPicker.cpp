// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

#include "Features/Engine/Picker/KLDebugUtilsPickerScoredObjects.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "CollisionQueryParams.h"         //trace picker
#include "Engine/EngineTypes.h"           //trace picker
#include "Engine/GameViewportClient.h"    //trace picker
#include "Engine/LocalPlayer.h"           //trace picker
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Box.h"
#include "Math/Matrix.h"    //trace picker
#include "Math/Rotator.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector2D.h"    //trace picker
#include "SceneView.h"        //trace picker
#include "UnrealClient.h"     //trace picker
#include "UObject/Object.h"

namespace KL::Debug::Utils::Picker
{
    template <typename Callback>
    void GatherAllObjects(const UWorld& _World, const TSubclassOf<AActor>& _ActorType, TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects, const Callback& _Callback)
    {
        float Distance = 0.f;
        for (TActorIterator<AActor> It(&_World, _ActorType); It; ++It)
        {
            AActor* Actor = *It;
            if (_Callback(*Actor, Distance))
            {
                continue;
            }

            _OutObjects.Emplace(Distance, *Actor);
        }
    }
}    // namespace KL::Debug::Utils::Picker

///////////////////////////////////////////////////////////////////////////

FKLDebugUtilsPicker::FKLDebugUtilsPicker()
    : mActorType(AActor::StaticClass())
{
}

UObject* FKLDebugUtilsPicker::GetPickedObject(const UWorld& _World) const
{
    TArray<FKLDebugUtilsPickerScoredObjects> Objects;
    GatherAllObjects(_World, Objects);
    if (Objects.IsEmpty())
    {
        return nullptr;
    }

    SortByDistance(Objects);
    ApplyScores(Objects);
    return Objects[0].GetScore() == 0.f ? nullptr : Objects[0].GetObject();
}

void FKLDebugUtilsPicker::SetActorType(UClass& _Class)
{
    mActorType = &_Class;
}

void FKLDebugUtilsPicker::SetInterfaceType(UClass& _Class)
{
    mActorInterface = &_Class;
}

bool FKLDebugUtilsPicker::GetCenterLocationFromScreenCoord(const UWorld& _World, FVector& _OutLocation, FVector& _OutDirecton) const
{
    const APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(&_World, 0);
    if (!PlayerCamera)
    {
        return false;
    }

    FRotator CamRot;
    PlayerCamera->GetCameraViewPoint(_OutLocation, CamRot);
    _OutDirecton = UKismetMathLibrary::GetForwardVector(CamRot);
    return true;
}

UObject* FKLDebugUtilsPicker::GetActorFromTrace(const UWorld& _World) const
{
    const ULocalPlayer* LP = _World.GetFirstLocalPlayerFromController();
    if (!LP || !LP->ViewportClient)
    {
        return nullptr;
    }

    FSceneViewProjectionData ProjectionData;
    if (!LP->GetProjectionData(LP->ViewportClient->Viewport, ProjectionData))
    {
        return nullptr;
    }

    ensureMsgf(false, TEXT("TODO"));

    // TODO
    const FVector2D ScreenPos = FVector2D::ZeroVector;
    const FMatrix InvViewProjMatrix = ProjectionData.ComputeViewProjectionMatrix().InverseFast();

    FVector WorldPosition, WorldDirection;
    FSceneView::DeprojectScreenToWorld(ScreenPos, ProjectionData.GetConstrainedViewRect(), InvViewProjMatrix, WorldPosition, WorldDirection);

    FCollisionQueryParams Params("GetActorFromTrace", SCENE_QUERY_STAT_ONLY(KLDebugUtilsPicker), true);

    FCollisionObjectQueryParams ObjectParams(
    ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic) | ECC_TO_BITFIELD(ECC_Pawn) | ECC_TO_BITFIELD(ECC_PhysicsBody));

    UObject* PickedActor = nullptr;
    FHitResult OutHit;
    const bool Result = _World.LineTraceSingleByObjectType(
    OutHit,
    WorldPosition + WorldDirection * 100.0,
    WorldPosition + WorldDirection * 10000.0,
    ObjectParams,
    Params);

    if (Result)
    {
        PickedActor = OutHit.GetActor();
    }

    return PickedActor;
}

void FKLDebugUtilsPicker::GatherAllObjects(const UWorld& _World, TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const
{
    FVector Location;
    FVector Directon;
    if (!GetCenterLocationFromScreenCoord(_World, Location, Directon))
    {
        return;
    }

    _OutObjects.Reserve(1000);

    if (mActorTag.IsNone() && mActorInterface.Get() == nullptr)
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool {
            return IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }
    else if (!mActorTag.IsNone() && mActorInterface.Get() != nullptr)
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool {
            return _Actor.ActorHasTag(mActorTag) && _Actor.GetClass()->ImplementsInterface(mActorInterface.Get()) && IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }
    else if (!mActorTag.IsNone())
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool {
            return _Actor.ActorHasTag(mActorTag) && IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }
    else
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool {
            return _Actor.GetClass()->ImplementsInterface(mActorInterface.Get()) && IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }

    _OutObjects.Shrink();
}

bool FKLDebugUtilsPicker::IsRightDistance(const AActor& _Actor, const FVector& _Position, const FVector& _Direction, float& _OutDistance) const
{
    FVector Pos;
    FVector Extents;
    _Actor.GetActorBounds(true, Pos, Extents);
    // const FBox Box{Pos - Extents, Pos + Extents};

    // FMath::LineBoxIntersection(InBox, RootPos, TipPos, TipPos - RootPos)
    // Box.ComputeSquaredDistanceToPoint()

    // NOTE THIS IS WRONG: need to transform the obejcts in screen space.
    // also need to take into account the bounding box of the actor (is any)
    /*
    * APlayerController * PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    FVector2D FinalLocation;
    FVector2D viewportDimensions;
    FVector2D ResultVector;

    float ResultLength;
    int viewportX;
    int viewportY;


    PlayerController-&gt;GetViewportSize(viewportX,viewportY);
    viewportDimensions = FVector2D(viewportX, viewportY);
    PlayerController-&gt;ProjectWorldLocationToScreen(Pos, FinalLocation);

    ResultVector = (viewportDimensions/2) - FinalLocation;
    ResultLength = ResultVector.Size();
    */
    _OutDistance = FMath::PointDistToLine(Pos, _Direction, _Position);
    return _OutDistance < mMaxDistance;
}

void FKLDebugUtilsPicker::SortByDistance(TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const
{
    _OutObjects.Sort([](const FKLDebugUtilsPickerScoredObjects& _Left, const FKLDebugUtilsPickerScoredObjects& _Right) -> bool {
        return _Left.GetDistance() < _Right.GetDistance();
    });
}

void FKLDebugUtilsPicker::ApplyScores(TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const
{
    // TODO: actual make scoring based on user data
    if (!_OutObjects.IsEmpty())
    {
        _OutObjects[0].SetScore(1.f);
    }

    // for (FKLDebugUtilsPickerScoredObjects& Object : _OutObjects)
    //{
    //     Object.SetScore(1.f);
    // }

    //_OutObjects.Sort([](const FKLDebugUtilsPickerScoredObjects& _Left, const FKLDebugUtilsPickerScoredObjects& _Right) -> bool {
    //    return _Left.GetScore() < _Right.GetScore();
    //});
}
