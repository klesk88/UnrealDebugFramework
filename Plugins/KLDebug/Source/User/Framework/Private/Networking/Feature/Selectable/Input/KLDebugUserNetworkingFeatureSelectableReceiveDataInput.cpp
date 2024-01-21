// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Networking/Feature/Selectable/Input/KLDebugUserNetworkingFeatureSelectableReceiveDataInput.h"

FKLDebugUserNetworkingFeatureSelectableReceiveDataInput::FKLDebugUserNetworkingFeatureSelectableReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, UObject& _Object, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureReceiveDataInputBase(_ReceiveType, _World, _FeatureContext, _Archive)
    , mObject(_Object)
{
}
