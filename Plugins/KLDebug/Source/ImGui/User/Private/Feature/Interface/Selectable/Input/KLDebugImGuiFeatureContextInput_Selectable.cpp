// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureContextInput_Selectable.h"

// engine
#include "GameFramework/Actor.h"

FKLDebugImGuiFeatureContextInput_Selectable::FKLDebugImGuiFeatureContextInput_Selectable(const ENetMode _CurrentNetMode, const UObject& _Object)
    : FKLDebugImGuiFeatureContextInput_Base(_CurrentNetMode)
    , mObject(_Object)
{
    if (const AActor* Actor = Cast<const AActor>(&_Object))
    {
        mHasAutorithyOnObject = Actor->HasAuthority();
    }
}
