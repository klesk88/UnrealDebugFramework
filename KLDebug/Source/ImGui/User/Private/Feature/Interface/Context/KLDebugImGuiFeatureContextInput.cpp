#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"

//engine
#include "GameFramework/Actor.h"

FKLDebugImGuiFeatureContextInput::FKLDebugImGuiFeatureContextInput(const ENetMode _CurrentNetMode, const UObject& _Object)
    : mCurrentNetMode(_CurrentNetMode)
    , mObject(_Object)
{
    if (const AActor* Actor = Cast<const AActor>(&_Object))
    {
        mHasAutorithyOnObject = Actor->HasAuthority();
    }
}
