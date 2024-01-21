// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Interface/Input/KLDebugFeatureRenderInput_Base.h"

// engine
#include "Engine/World.h"

FKLDebugFeatureRenderInput_Base::FKLDebugFeatureRenderInput_Base(const UWorld& _World, IKLDebugContextInterface* _Context)
    : mWorld(_World)
    , mContextData(_Context)
{
}
