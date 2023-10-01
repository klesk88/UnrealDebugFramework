#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureStatusUpdateData
{
public:
    explicit FKLDebugImGuiFeatureStatusUpdateData(const bool _IsAdded, const EContainerType _ContainerType, const UObject& _Object, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator);
    explicit FKLDebugImGuiFeatureStatusUpdateData(const bool _IsAdded, const EContainerType _ContainerType, const FObjectKey& _ObjectKey, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator);

    void SetFullyRemoved();
    UE_NODISCARD bool IsFullyRemoved() const;

    UE_NODISCARD bool IsFeatureAdded() const;
    UE_NODISCARD FKLDebugImGuiSubsetFeaturesConstIterator& GetFeatureIterator() const;
    UE_NODISCARD EContainerType GetContainerType() const;
    UE_NODISCARD const UObject* TryGetObject() const;
    UE_NODISCARD const FObjectKey& GetObjectKey() const;

private:
    FKLDebugImGuiSubsetFeaturesConstIterator& mFeaturesIterator;
    const UObject* mObject = nullptr;
    FObjectKey mObjectKey;
    bool mIsAdded = false;
    bool mFullyRemove = false;
    EContainerType mContainerType = EContainerType::COUNT;
};

inline bool FKLDebugImGuiFeatureStatusUpdateData::IsFeatureAdded() const
{
    return mIsAdded;
}

inline FKLDebugImGuiSubsetFeaturesConstIterator& FKLDebugImGuiFeatureStatusUpdateData::GetFeatureIterator() const
{
    return mFeaturesIterator;
}

inline EContainerType FKLDebugImGuiFeatureStatusUpdateData::GetContainerType() const
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