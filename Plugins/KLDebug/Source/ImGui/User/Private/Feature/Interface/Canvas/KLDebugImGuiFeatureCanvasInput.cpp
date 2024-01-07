// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Canvas/KLDebugImGuiFeatureCanvasInput.h"

FKLDebugImGuiFeatureCanvasInput::FKLDebugImGuiFeatureCanvasInput(UCanvas& _Canvas, UFont& _Font)
    : FGameplayDebuggerCanvasContext(&_Canvas, &_Font)
{
}