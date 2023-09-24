#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"

FKLDebugImGuiFeatureStatusUpdateData::FKLDebugImGuiFeatureStatusUpdateData(const bool _IsAdded, const EContainerType _ContainerType, const UObject& _Object, FKLDebugImGuiSubsetFeaturesConstIterator& _FeaturesUpdatedIterator)
    : mFeaturesIterator(_FeaturesUpdatedIterator)
    , mObject(_Object)
    , mIsAdded(_IsAdded)
    , mContainerType(_ContainerType)
{
}
