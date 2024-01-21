// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureCommonInputBase.h"

// engine
#include "Serialization/Archive.h"

class UWorld;

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureServerTickInputBase : public FKLDebugUserNetworkingFeatureCommonInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureServerTickInputBase(const UWorld& _World, IKLDebugContextInterface* _ContextData, FArchive& _Archive);

    UE_NODISCARD FArchive& GetArchiveMutable() const;

private:
    FArchive& mArchive;
};

inline FArchive& FKLDebugUserNetworkingFeatureServerTickInputBase::GetArchiveMutable() const
{
    return mArchive;
}
