#include "Feature/Container/KLDebugImGuiFeaturesIterator.h"

#include "Feature/Interface/KLDebugImGuiFeatureInterface.h"

FKLDebugImGuiFeaturesIterator::FKLDebugImGuiFeaturesIterator(const TArray<uint32>& _FeaturesOffset, TArray<uint8>& _FeaturesPool)
    : mFeaturesOffset(_FeaturesOffset)
    , mFeaturesPool(_FeaturesPool)
{
}

IKLDebugImGuiFeatureInterface& FKLDebugImGuiFeaturesIterator::GetFeatureMutable() const
{
    const uint32 Offset = mFeaturesOffset[mIndex];
    return *reinterpret_cast<IKLDebugImGuiFeatureInterface*>(&mFeaturesPool[Offset]);
}
