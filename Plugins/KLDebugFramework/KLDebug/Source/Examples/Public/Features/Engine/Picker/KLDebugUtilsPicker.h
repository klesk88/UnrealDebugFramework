// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "Templates/SubclassOf.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class AActor;
class FKLDebugUtilsPickerScoredObjects;
class UClass;
class UInterface;
class UObject;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugUtilsPicker final : public FNoncopyable
{
public:
    FKLDebugUtilsPicker();

    UE_NODISCARD UObject* GetPickedObject(const UWorld& _World) const;

    void SetActorType(UClass& _Class);
    void SetInterfaceType(UClass& _Class);
    void SetActorTag(const FName& _Tag);
    void SetMaxDistanceFromScreenCenter(const float _MaxDistance);

private:
    UE_NODISCARD UObject* GetActorFromTrace(const UWorld& _World) const;

    void GatherAllObjects(const UWorld& _World, TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const;
    UE_NODISCARD bool GetCenterLocationFromScreenCoord(const UWorld& _World, FVector& _Location, FVector& _OutDirecton) const;
    UE_NODISCARD bool IsRightDistance(const AActor& _Actor, const FVector& _Position, const FVector& _Direction, float& _OutDistance) const;

    void SortByDistance(TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const;
    void ApplyScores(TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const;

private:
    TSubclassOf<AActor> mActorType;
    TSubclassOf<UInterface> mActorInterface;
    FName mActorTag;
    float mMaxDistance = 10.f;
};

inline void FKLDebugUtilsPicker::SetActorTag(const FName& _Tag)
{
    mActorTag = _Tag;
}

inline void FKLDebugUtilsPicker::SetMaxDistanceFromScreenCenter(const float _MaxDistance)
{
    mMaxDistance = _MaxDistance;
}