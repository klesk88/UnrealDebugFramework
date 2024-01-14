// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureCommonInputBase.h"

// engine
#include "Serialization/Archive.h"
#include "Templates/UnrealTemplate.h"

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureReceiveDataInputBase : public FKLDebugUserNetworkingFeatureCommonInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureReceiveDataInputBase(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive);

    UE_NODISCARD FArchive& GetArchiveMutable() const;

private:
    FArchive& mArchive;
};

inline FArchive& FKLDebugUserNetworkingFeatureReceiveDataInputBase::GetArchiveMutable() const
{
    return mArchive;
}
