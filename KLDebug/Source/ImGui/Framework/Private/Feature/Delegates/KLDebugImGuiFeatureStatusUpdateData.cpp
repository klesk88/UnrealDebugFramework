#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"

FKLDebugImGuiFeatureStatusUpdateData::FKLDebugImGuiFeatureStatusUpdateData(const UWorld& _World, const bool _IsAdded, const EImGuiInterfaceType _ContainerType, const UObject& _Object, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator)
    : mFeaturesIterator(_FeaturesUpdatedIterator)
    , mWorld(_World)
    , mObject(&_Object)
    , mObjectKey(&_Object)
    , mIsAdded(_IsAdded)
    , mContainerType(_ContainerType)
{
}

FKLDebugImGuiFeatureStatusUpdateData::FKLDebugImGuiFeatureStatusUpdateData(const UWorld& _World, const bool _IsAdded, const EImGuiInterfaceType _ContainerType, const FObjectKey& _ObjectKey, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator)
    : mFeaturesIterator(_FeaturesUpdatedIterator)
    , mWorld(_World)
    , mObjectKey(_ObjectKey)
    , mIsAdded(_IsAdded)
    , mContainerType(_ContainerType)
{
}
