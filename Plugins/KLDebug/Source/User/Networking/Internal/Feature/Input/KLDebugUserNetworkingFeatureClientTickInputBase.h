// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureCommonInputBase.h"

// engine
#include "Serialization/Archive.h"

class FKLDebugImGuiFeatureContext_Base;
class UWorld;

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureClientTickInputBase : public FKLDebugUserNetworkingFeatureCommonInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureClientTickInputBase(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _Context, FArchive& _Archive);
    virtual ~FKLDebugUserNetworkingFeatureClientTickInputBase() = default;

    UE_NODISCARD FArchive& GetWriter() const;

    void SetUpdateSceneProxy();
    UE_NODISCARD bool ShouldUpdateSceneProxy() const;

private:
    FArchive& mArchive;
    bool mUpdateSceneProxy = false;
};

inline FArchive& FKLDebugUserNetworkingFeatureClientTickInputBase::GetWriter() const
{
    return mArchive;
}

inline void FKLDebugUserNetworkingFeatureClientTickInputBase::SetUpdateSceneProxy()
{
    mUpdateSceneProxy = true;
}

inline bool FKLDebugUserNetworkingFeatureClientTickInputBase::ShouldUpdateSceneProxy() const
{
    return mUpdateSceneProxy;
}