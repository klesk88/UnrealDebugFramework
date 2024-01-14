// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Mode/Input/KLDebugModeDrawCanvasInput.h"

FKLDebugModeDrawCanvasInput::FKLDebugModeDrawCanvasInput(const UWorld& _World, UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context)
    : FGameplayDebuggerCanvasContext(&_Canvas, &_Font)
    , mWorld(_World)
    , mContext(_Context)
{
}
