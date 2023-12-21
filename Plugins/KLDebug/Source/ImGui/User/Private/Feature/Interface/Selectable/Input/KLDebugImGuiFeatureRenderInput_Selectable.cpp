// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureRenderInput_Selectable.h"

// engine
#include "UObject/Object.h"

FKLDebugImGuiFeatureRenderInput_Selectable::FKLDebugImGuiFeatureRenderInput_Selectable(const UWorld& _World, const UObject& _Object)
    : FKLDebugImGuiFeatureRenderInput_Base(_World)
    , mObject(_Object)
{
}
