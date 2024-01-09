// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

// engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureStatusUpdateData
{
public:
    explicit FKLDebugImGuiFeatureStatusUpdateData(const UWorld& _World, const bool _IsAdded, const EImGuiInterfaceType _ContainerType, const UObject& _Object, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator);
    explicit FKLDebugImGuiFeatureStatusUpdateData(const UWorld& _World, const bool _IsAdded, const EImGuiInterfaceType _ContainerType, const FObjectKey& _ObjectKey, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator);

    UE_NODISCARD const UWorld& GetOwnerWorld() const;

    void SetFullyRemoved();
    UE_NODISCARD bool IsFullyRemoved() const;

    UE_NODISCARD bool IsFeatureAdded() const;
    UE_NODISCARD FKLDebugImGuiSubsetFeaturesConstIterator& GetFeatureIterator() const;
    UE_NODISCARD EImGuiInterfaceType GetContainerType() const;
    UE_NODISCARD const UObject* TryGetObject() const;
    UE_NODISCARD const FObjectKey& GetObjectKey() const;

private:
    FKLDebugImGuiSubsetFeaturesConstIterator& mFeaturesIterator;
    const UWorld& mWorld;
    const UObject* mObject = nullptr;
    FObjectKey mObjectKey;
    bool mIsAdded = false;
    bool mFullyRemove = false;
    EImGuiInterfaceType mContainerType = EImGuiInterfaceType::COUNT;
};

inline const UWorld& FKLDebugImGuiFeatureStatusUpdateData::GetOwnerWorld() const
{
    return mWorld;
}

inline bool FKLDebugImGuiFeatureStatusUpdateData::IsFeatureAdded() const
{
    return mIsAdded;
}

inline FKLDebugImGuiSubsetFeaturesConstIterator& FKLDebugImGuiFeatureStatusUpdateData::GetFeatureIterator() const
{
    // we need to reset the index in the case of multiple callbacks
    mFeaturesIterator.ResetIndex();
    return mFeaturesIterator;
}

inline EImGuiInterfaceType FKLDebugImGuiFeatureStatusUpdateData::GetContainerType() const
{
    return mContainerType;
}

inline const UObject* FKLDebugImGuiFeatureStatusUpdateData::TryGetObject() const
{
    return mObject;
}

inline const FObjectKey& FKLDebugImGuiFeatureStatusUpdateData::GetObjectKey() const
{
    return mObjectKey;
}

inline void FKLDebugImGuiFeatureStatusUpdateData::SetFullyRemoved()
{
    mFullyRemove = true;
}

inline bool FKLDebugImGuiFeatureStatusUpdateData::IsFullyRemoved() const
{
    return mFullyRemove;
}