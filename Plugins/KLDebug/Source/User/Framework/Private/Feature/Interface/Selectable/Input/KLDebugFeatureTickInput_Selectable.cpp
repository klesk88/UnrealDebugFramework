// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/Input/KLDebugFeatureTickInput_Selectable.h"

FKLDebugFeatureTickInput_Selectable::FKLDebugFeatureTickInput_Selectable(const UWorld& _World, const UObject& _Owner, IKLDebugContextInterface* _Context)
    : FKLDebugFeatureTickInput_Base(_World, _Context)
    , mOwner(_Owner)
{
}
