#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Selectable.h"

FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext, UObject& _Object)
    : FKLDebugImGuiFeatureInterfaceImGuiInput_Base(_World, _IsWindowOpen, _FeatureContext)
    , mObject(_Object)
{
}
