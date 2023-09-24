#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"

FKLDebugImGuiFeatureContextInput::FKLDebugImGuiFeatureContextInput(const ENetMode _CurrentNetMode, const UObject& _OptionalObject)
    : mCurrentNetMode(_CurrentNetMode)
    , mObject(_OptionalObject)
{
}
