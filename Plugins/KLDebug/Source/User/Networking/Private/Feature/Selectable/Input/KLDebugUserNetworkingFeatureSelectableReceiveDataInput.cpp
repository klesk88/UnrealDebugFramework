// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Selectable/Input/KLDebugUserNetworkingFeatureSelectableReceiveDataInput.h"

FKLDebugUserNetworkingFeatureSelectableReceiveDataInput::FKLDebugUserNetworkingFeatureSelectableReceiveDataInput(const UWorld& _World, UObject& _Object, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureReceiveDataInputBase(_World, _FeatureContext, _Archive)
    , mObject(_Object)
{
}
