// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Unique/Input/KLDebugUserNetworkingFeatureUniqueReceiveDataInput.h"

FKLDebugUserNetworkingFeatureUniqueReceiveDataInput::FKLDebugUserNetworkingFeatureUniqueReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureReceiveDataInputBase(_ReceiveType, _World, _FeatureContext, _Archive)
{
}
