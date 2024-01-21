// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureClientTickInputBase.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureUniqueClientTickInput final : public FKLDebugUserNetworkingFeatureClientTickInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureUniqueClientTickInput(const UWorld& _World, IKLDebugContextInterface* _Context, FArchive& _Archive);
};
