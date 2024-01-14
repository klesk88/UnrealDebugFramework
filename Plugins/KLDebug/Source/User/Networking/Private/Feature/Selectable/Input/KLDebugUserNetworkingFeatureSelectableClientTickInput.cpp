// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Selectable/Input/KLDebugUserNetworkingFeatureSelectableClientTickInput.h"

FKLDebugUserNetworkingFeatureSelectableClientTickInput::FKLDebugUserNetworkingFeatureSelectableClientTickInput(const UWorld& _World, UObject& _OwnerObject, FKLDebugImGuiFeatureContext_Base* _Context, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureClientTickInputBase(_World, _Context, _Archive)
    , mObject(_OwnerObject)
{
}
