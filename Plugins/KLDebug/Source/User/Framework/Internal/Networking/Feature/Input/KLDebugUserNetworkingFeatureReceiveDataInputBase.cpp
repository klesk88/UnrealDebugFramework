// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureReceiveDataInputBase.h"

FKLDebugUserNetworkingFeatureReceiveDataInputBase::FKLDebugUserNetworkingFeatureReceiveDataInputBase(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureCommonInputBase(_World, _FeatureContext)
    , mArchive(_Archive)
    , mReceiveType(_ReceiveType)
{
}
