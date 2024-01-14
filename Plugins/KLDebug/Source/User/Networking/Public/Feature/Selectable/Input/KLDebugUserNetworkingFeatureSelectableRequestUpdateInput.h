// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureRequestUpdateInputBase.h"

class UObject;

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput final : public FKLDebugUserNetworkingFeatureRequestUpdateInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput(const UWorld& _World, const UObject& _Object, FArchive& _ReaderArchive, FArchive& _WriterArchive);

    UE_NODISCARD const UObject& GetObject() const;

private:
    const UObject& mObject;
};

inline const UObject& FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput::GetObject() const
{
    return mObject;
}
