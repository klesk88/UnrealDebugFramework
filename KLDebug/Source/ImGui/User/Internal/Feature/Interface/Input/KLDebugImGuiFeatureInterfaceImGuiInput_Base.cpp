#include "Feature/Interface/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Base.h"

//engine
#include "Engine/World.h"

FKLDebugImGuiFeatureInterfaceImGuiInput_Base::FKLDebugImGuiFeatureInterfaceImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext)
    : mWorld(_World)
    , mFeatureContext(_FeatureContext)
    , mIsWindowOpen(_IsWindowOpen)
{
}
