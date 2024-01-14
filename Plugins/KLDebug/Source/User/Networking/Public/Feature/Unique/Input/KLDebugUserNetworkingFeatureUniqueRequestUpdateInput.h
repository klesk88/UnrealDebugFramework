// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureRequestUpdateInputBase.h"

class UObject;

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput final : public FKLDebugUserNetworkingFeatureRequestUpdateInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput(const UWorld& _World, FArchive& _ReaderArchive, FArchive& _WriterArchive);
};
