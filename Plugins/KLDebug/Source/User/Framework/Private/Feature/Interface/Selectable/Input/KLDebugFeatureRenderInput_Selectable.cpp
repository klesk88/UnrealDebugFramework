// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/Input/KLDebugFeatureRenderInput_Selectable.h"

// engine
#include "UObject/Object.h"

FKLDebugFeatureRenderInput_Selectable::FKLDebugFeatureRenderInput_Selectable(const UWorld& _World, const UObject& _Object, IKLDebugContextInterface* _Context)
    : FKLDebugFeatureRenderInput_Base(_World, _Context)
    , mObject(_Object)
{
}
