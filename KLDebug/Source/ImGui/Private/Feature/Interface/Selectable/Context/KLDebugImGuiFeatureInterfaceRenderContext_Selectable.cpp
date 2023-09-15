#include "Feature/Interface/Selectable/Context/KLDebugImGuiFeatureInterfaceRenderContext_Selectable.h"

FKLDebugImGuiFeatureInterfaceRenderContext_Selectable::FKLDebugImGuiFeatureInterfaceRenderContext_Selectable(const UWorld& _World, const UObject& _Object)
    : FKLDebugImGuiFeatureInterfaceRenderContext_Base(_World)
    , mObject(_Object)
{
}
