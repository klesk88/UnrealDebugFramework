// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Networking/Feature/Unique/Input/KLDebugUserNetworkingFeatureUniqueReceiveDataInput.h"

FKLDebugUserNetworkingFeatureUniqueReceiveDataInput::FKLDebugUserNetworkingFeatureUniqueReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureReceiveDataInputBase(_ReceiveType, _World, _FeatureContext, _Archive)
{
}
