// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Unique/Input/KLDebugImGuiFeatureContextInput_Unique.h"

// engine
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

FKLDebugImGuiFeatureContextInput_Unique::FKLDebugImGuiFeatureContextInput_Unique(const ENetMode _CurrentNetMode, const UWorld& _World)
    : FKLDebugImGuiFeatureContextInput_Base(_CurrentNetMode)
    , mWorld(_World)
{
    mHasAutorithyOnObject = UKismetSystemLibrary::IsServer(&_World);
}
