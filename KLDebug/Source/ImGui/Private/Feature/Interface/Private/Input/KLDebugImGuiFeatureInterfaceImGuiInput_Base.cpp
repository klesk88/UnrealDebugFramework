#include "Feature/Interface/Private/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Base.h"

//engine
#include "Engine/World.h"

FKLDebugImGuiFeatureInterfaceImGuiInput_Base::FKLDebugImGuiFeatureInterfaceImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen)
    : mWorld(_World)
    , mIsWindowOpen(_IsWindowOpen)
{
}
