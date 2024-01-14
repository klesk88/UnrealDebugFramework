// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureClientTickInputBase.h"

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureUniqueClientTickInput final : public FKLDebugUserNetworkingFeatureClientTickInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureUniqueClientTickInput(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _Context, FArchive& _Archive);
};
