// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureCommonInputBase.h"

// engine
#include "Serialization/Archive.h"

class UWorld;

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureServerTickInputBase : public FKLDebugUserNetworkingFeatureCommonInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureServerTickInputBase(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive);

    UE_NODISCARD FArchive& GetArchiveMutable() const;

private:
    FArchive& mArchive;
};

inline FArchive& FKLDebugUserNetworkingFeatureServerTickInputBase::GetArchiveMutable() const
{
    return mArchive;
}
