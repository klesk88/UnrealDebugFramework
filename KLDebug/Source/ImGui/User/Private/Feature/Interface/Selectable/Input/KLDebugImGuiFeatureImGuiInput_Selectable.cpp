#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureImGuiInput_Selectable.h"

//engine
#include "GameFramework/Actor.h"

FKLDebugImGuiFeatureImGuiInput_Selectable::FKLDebugImGuiFeatureImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext, UObject& _Object)
    : FKLDebugImGuiFeatureImGuiInput_Base(_World, _IsWindowOpen)
    , mObject(_Object)
    , mContextData(_FeatureContext)
{
    if (const AActor* Actor = Cast<const AActor>(&_Object))
    {
        mHasAuthorityOnObject = Actor->HasAuthority();
    }
}
