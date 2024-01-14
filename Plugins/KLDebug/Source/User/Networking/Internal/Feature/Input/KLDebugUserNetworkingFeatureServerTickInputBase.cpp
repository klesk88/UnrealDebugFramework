// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureServerTickInputBase.h"

FKLDebugUserNetworkingFeatureServerTickInputBase::FKLDebugUserNetworkingFeatureServerTickInputBase(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive)
    : FKLDebugUserNetworkingFeatureCommonInputBase(_World, _ContextData)
    , mArchive(_Archive)
{
}
