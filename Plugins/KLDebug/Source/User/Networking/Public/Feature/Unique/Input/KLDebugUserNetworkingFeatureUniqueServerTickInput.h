// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureServerTickInputBase.h"

class UObject;

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureUniqueServerTickInput final : public FKLDebugUserNetworkingFeatureServerTickInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureUniqueServerTickInput(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive);
};
