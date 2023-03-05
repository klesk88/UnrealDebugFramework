#include "Feature/Container/KLDebugImGuiFeaturesIterator.h"

FKLDebugImGuiFeaturesIterator::FKLDebugImGuiFeaturesIterator(const TArray<KL::Debug::Features::Types::FeatureIndex>& _FeaturesOffset, TArray<uint8>& _FeaturesPool)
    : mFeaturesOffset(_FeaturesOffset)
    , mFeaturesPool(_FeaturesPool)
{
}
