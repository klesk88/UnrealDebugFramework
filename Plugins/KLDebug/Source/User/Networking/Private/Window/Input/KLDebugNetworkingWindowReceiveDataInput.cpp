// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Window/Input/KLDebugNetworkingWindowReceiveDataInput.h"

FKLDebugNetworkingWindowReceiveDataInput::FKLDebugNetworkingWindowReceiveDataInput(const EKLDebugImGuiNetworkReceiveMessageType _ReceiveMessageType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive)
    : FKLDebugNetworkingWindowInputBase(_World, _FeatureContext, _Archive)
    , mReceiveMessageType(_ReceiveMessageType)
{
}
