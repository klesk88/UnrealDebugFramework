#include "Feature/Interface/Selectable/Context/KLDebugImGuiFeatureInterfaceImGuiContext_Selectable.h"

FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable::FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable(const UWorld& _World, bool& _IsWindowOpen, UObject& _Object)
    : FKLDebugImGuiFeatureInterfaceImGuiContext_Base(_World, _IsWindowOpen)
    , mObject(_Object)
{
}
