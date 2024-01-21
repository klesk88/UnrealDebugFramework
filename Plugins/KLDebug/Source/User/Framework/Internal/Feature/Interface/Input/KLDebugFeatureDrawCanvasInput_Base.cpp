// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Interface/Input/KLDebugFeatureDrawCanvasInput_Base.h"

FKLDebugFeatureDrawCanvasInput_Base::FKLDebugFeatureDrawCanvasInput_Base(UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context)
    : FGameplayDebuggerCanvasContext(&_Canvas, &_Font)
    , mContext(_Context)
{
}
