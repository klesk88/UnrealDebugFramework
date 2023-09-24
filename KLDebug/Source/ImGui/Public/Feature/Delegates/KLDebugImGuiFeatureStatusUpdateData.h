#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureStatusUpdateData
{
public:
    explicit FKLDebugImGuiFeatureStatusUpdateData(const bool _IsAdded, const EContainerType _ContainerType, const UObject& _Object, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator);

    UE_NODISCARD bool IsFeatureAdded() const;
    UE_NODISCARD FKLDebugImGuiSubsetFeaturesConstIterator& GetFeatureIterator() const;
    UE_NODISCARD EContainerType GetContainerType() const;
    UE_NODISCARD const UObject& GetObject() const;

private:
    FKLDebugImGuiSubsetFeaturesConstIterator& mFeaturesIterator;
    const UObject& mObject;
    bool mIsAdded = false;
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

inline const UObject& FKLDebugImGuiFeatureStatusUpdateData::GetObject() const
{
    return mObject;
}