// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Context/Input/KLDebugContextGetterInput.h"

FKLDebugContextGetterInput::FKLDebugContextGetterInput(const UWorld& _World, const ENetMode _CurrentNetMode)
    : mWorld(_World)
    , mCurrentNetMode(_CurrentNetMode)
{
}
