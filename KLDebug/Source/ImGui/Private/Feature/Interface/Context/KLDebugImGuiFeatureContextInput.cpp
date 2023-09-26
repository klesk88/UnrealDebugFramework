#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"

FKLDebugImGuiFeatureContextInput::FKLDebugImGuiFeatureContextInput(const ENetMode _CurrentNetMode, const UObject& _Object)
    : mCurrentNetMode(_CurrentNetMode)
    , mObject(_Object)
{
}
