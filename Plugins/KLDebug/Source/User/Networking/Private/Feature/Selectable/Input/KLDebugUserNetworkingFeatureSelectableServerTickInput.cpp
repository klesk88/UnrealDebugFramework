// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Selectable/Input/KLDebugUserNetworkingFeatureSelectableServerTickInput.h"

FKLDebugUserNetworkingFeatureSelectableServerTickInput::FKLDebugUserNetworkingFeatureSelectableServerTickInput(const UWorld& _World, const bool _HasAuthority, UObject& _OwnerObject, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureServerTickInputBase(_World, _ContextData, _Archive)
    , mObject(_OwnerObject)
    , mHasAuthotityOnObject(_HasAuthority)
{
}
