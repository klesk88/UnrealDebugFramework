#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Selectable.h"

FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, UObject& _Object)
    : FKLDebugImGuiFeatureInterfaceImGuiInput_Base(_World, _IsWindowOpen)
    , mObject(_Object)
{
}
