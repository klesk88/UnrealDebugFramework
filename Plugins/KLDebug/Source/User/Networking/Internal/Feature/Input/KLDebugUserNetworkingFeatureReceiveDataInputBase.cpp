// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureReceiveDataInputBase.h"

FKLDebugUserNetworkingFeatureReceiveDataInputBase::FKLDebugUserNetworkingFeatureReceiveDataInputBase(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureCommonInputBase(_World, _FeatureContext)
    , mArchive(_Archive)
    , mReceiveType(_ReceiveType)
{
}
