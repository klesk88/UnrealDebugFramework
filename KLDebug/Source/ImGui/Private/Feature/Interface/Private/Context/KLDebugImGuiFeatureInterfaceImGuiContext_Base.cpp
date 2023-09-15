#include "Feature/Interface/Private/Context/KLDebugImGuiFeatureInterfaceImGuiContext_Base.h"

//engine
#include "Engine/World.h"

FKLDebugImGuiFeatureInterfaceImGuiContext_Base::FKLDebugImGuiFeatureInterfaceImGuiContext_Base(const UWorld& _World, bool& _IsWindowOpen)
    : mWorld(_World)
    , mIsWindowOpen(_IsWindowOpen)
{
}
