// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/Engine/Picker/KLDebugUtilsPickerScoredObjects.h"

FKLDebugUtilsPickerScoredObjects::FKLDebugUtilsPickerScoredObjects(const float _Distance, UObject& _Object)
    : mObject(&_Object)
    , mDistance(_Distance)
{
}
