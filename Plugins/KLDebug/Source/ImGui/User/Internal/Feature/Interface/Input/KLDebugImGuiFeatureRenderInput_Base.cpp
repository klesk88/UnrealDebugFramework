// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Input/KLDebugImGuiFeatureRenderInput_Base.h"

// engine
#include "Engine/World.h"

FKLDebugImGuiFeatureRenderInput_Base::FKLDebugImGuiFeatureRenderInput_Base(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _Context)
    : mWorld(_World)
    , mContextData(_Context)
{
}
