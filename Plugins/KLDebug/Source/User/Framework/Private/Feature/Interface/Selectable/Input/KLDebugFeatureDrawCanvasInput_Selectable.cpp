// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/Input/KLDebugFeatureDrawCanvasInput_Selectable.h"

FKLDebugFeatureDrawCanvasInput_Selectable::FKLDebugFeatureDrawCanvasInput_Selectable(const UObject& _Owner, UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context)
    : FKLDebugFeatureDrawCanvasInput_Base(_Canvas, _Font, _Context)
    , mOwnerObject(_Owner)
{
}
