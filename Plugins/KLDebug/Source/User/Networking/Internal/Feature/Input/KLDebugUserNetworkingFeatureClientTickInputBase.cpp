// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureClientTickInputBase.h"

FKLDebugUserNetworkingFeatureClientTickInputBase::FKLDebugUserNetworkingFeatureClientTickInputBase(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _Context, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureCommonInputBase(_World, _Context)
    , mArchive(_Archive)
{
}
