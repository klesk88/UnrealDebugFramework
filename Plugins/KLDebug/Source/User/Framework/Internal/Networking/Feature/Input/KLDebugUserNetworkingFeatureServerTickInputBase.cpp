// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureServerTickInputBase.h"

FKLDebugUserNetworkingFeatureServerTickInputBase::FKLDebugUserNetworkingFeatureServerTickInputBase(const UWorld& _World, IKLDebugContextInterface* _ContextData, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureCommonInputBase(_World, _ContextData)
    , mArchive(_Archive)
{
}
