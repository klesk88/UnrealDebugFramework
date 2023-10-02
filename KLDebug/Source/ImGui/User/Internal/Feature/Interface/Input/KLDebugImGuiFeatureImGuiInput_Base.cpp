#include "Feature/Interface/Input/KLDebugImGuiFeatureImGuiInput_Base.h"

//engine
#include "Engine/World.h"

FKLDebugImGuiFeatureImGuiInput_Base::FKLDebugImGuiFeatureImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen)
    : mWorld(_World)
    , mIsWindowOpen(_IsWindowOpen)
{
}
