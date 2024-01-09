// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Input/KLDebugFeatureDrawCanvasInput_Base.h"

FKLDebugFeatureDrawCanvasInput_Base::FKLDebugFeatureDrawCanvasInput_Base(UCanvas& _Canvas, UFont& _Font, FKLDebugImGuiFeatureContext_Base* _Context)
    : FGameplayDebuggerCanvasContext(&_Canvas, &_Font)
    , mContext(_Context)
{
}
