// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureCommonInputBase.h"

FKLDebugUserNetworkingFeatureCommonInputBase::FKLDebugUserNetworkingFeatureCommonInputBase(const UWorld& _World, IKLDebugContextInterface* _Context)
    : mWorld(_World)
    , mContextData(_Context)
{
}
