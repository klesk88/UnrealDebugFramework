#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureInterfaceRenderInput_Selectable.h"

//engine
#include "UObject/Object.h"

FKLDebugImGuiFeatureInterfaceRenderInput_Selectable::FKLDebugImGuiFeatureInterfaceRenderInput_Selectable(const UWorld& _World, const UObject& _Object)
    : FKLDebugImGuiFeatureInterfaceRenderInput_Base(_World)
    , mObject(_Object)
{
}
