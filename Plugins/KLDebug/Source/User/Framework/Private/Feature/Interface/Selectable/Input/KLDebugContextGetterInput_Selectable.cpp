// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/Input/KLDebugContextGetterInput_Selectable.h"

FKLDebugContextGetterInput_Selectable::FKLDebugContextGetterInput_Selectable(const UObject& _Object, const bool _HasAuthority, const UWorld& _World, const ENetMode _CurrentNetMode)
    : FKLDebugContextGetterInput(_World, _CurrentNetMode)
    , mObject(_Object)
    , mHasAuthority(_HasAuthority)
{
}
