// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureCommonInputBase.h"

// engine
#include "Serialization/Archive.h"

class IKLDebugContextInterface;
class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureClientTickInputBase : public FKLDebugUserNetworkingFeatureCommonInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureClientTickInputBase(const UWorld& _World, IKLDebugContextInterface* _Context, FArchive& _Archive);
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