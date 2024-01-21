// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Networking/Feature/Selectable/Input/KLDebugUserNetworkingFeatureSelectableServerTickInput.h"

FKLDebugUserNetworkingFeatureSelectableServerTickInput::FKLDebugUserNetworkingFeatureSelectableServerTickInput(const UWorld& _World, const bool _HasAuthority, UObject& _OwnerObject, IKLDebugContextInterface* _ContextData, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureServerTickInputBase(_World, _ContextData, _Archive)
    , mObject(_OwnerObject)
    , mHasAuthotityOnObject(_HasAuthority)
{
}
