#include "Picker/KLDebugUtilsPickerScoredObjects.h"

FKLDebugUtilsPickerScoredObjects::FKLDebugUtilsPickerScoredObjects(const float _Distance, UObject& _Object)
    : mObject(&_Object)
    , mDistance(_Distance)
{
}
