// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/Input/KLDebugFeatureImGuiInput_Selectable.h"

// engine
#include "GameFramework/Actor.h"

FKLDebugFeatureImGuiInput_Selectable::FKLDebugFeatureImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, IKLDebugContextInterface* _FeatureContext, UObject& _Object)
    : FKLDebugFeatureImGuiInput_Base(_World, _IsWindowOpen, _FeatureContext)
    , mObject(_Object)
{
    if (const AActor* Actor = Cast<const AActor>(&_Object))
    {
        mHasAuthorityOnObject = Actor->HasAuthority();
    }
}
