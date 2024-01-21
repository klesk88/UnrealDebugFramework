// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureServerTickInputBase.h"

class UObject;

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureUniqueServerTickInput final : public FKLDebugUserNetworkingFeatureServerTickInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureUniqueServerTickInput(const UWorld& _World, IKLDebugContextInterface* _ContextData, FArchive& _Archive);
};
