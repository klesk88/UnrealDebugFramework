// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerConstIterator final : public FKLDebugImGuiFeaturesConstIterator
{
public:
    explicit FKLDebugImGuiFeatureVisualizerConstIterator(const TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesConstIterator::PoolType& _FeaturesPool);

    UE_NODISCARD const FKLDebugImGuiFeatureVisualizerEntry& GetEntryData() const;

private:
    // FKLDebugImGuiFeaturesConstIterator
    UE_NODISCARD bool IsValid() const final;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndexChild() const final;
    // FKLDebugImGuiFeaturesConstIterator

    const TArray<FKLDebugImGuiFeatureVisualizerEntry>& mFeaturesIndexes;
};

inline FKLDebugImGuiFeatureVisualizerConstIterator::FKLDebugImGuiFeatureVisualizerConstIterator(const TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesConstIterator::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesConstIterator(_FeatureData, _FeaturesPool)
    , mFeaturesIndexes(_FeaturesIndexes)
{
}

inline const FKLDebugImGuiFeatureVisualizerEntry& FKLDebugImGuiFeatureVisualizerConstIterator::GetEntryData() const
{
    return mFeaturesIndexes[mIndex];
}

inline bool FKLDebugImGuiFeatureVisualizerConstIterator::IsValid() const
{
    return mFeaturesIndexes.IsValidIndex(static_cast<int32>(mIndex));
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFeatureVisualizerConstIterator::GetFeatureDataIndexChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex FeatureDataIndex = mFeaturesIndexes[mIndex].GetFeatureIndex();
    return FeatureDataIndex;
}

//////////////////////////////////////////////////////////////////

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureVisualizerIterator final : public FKLDebugImGuiFeaturesIterator
{
public:
    explicit FKLDebugImGuiFeatureVisualizerIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes, typename FKLDebugImGuiFeaturesIterator::PoolType& _FeaturesPool);

    UE_NODISCARD FKLDebugImGuiFeatureVisualizerEntry& GetEntryDataMutable();

private:
    // FKLDebugImGuiFeaturesIterator
    UE_NODISCARD bool IsValid() const final;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndexChild() const final;
    // FKLDebugImGuiFeaturesIterator

private:
    TArray<FKLDebugImGuiFeatureVisualizerEntry>& mFeaturesIndexes;
};

inline FKLDebugImGuiFeatureVisualizerIterator::FKLDebugImGuiFeatureVisualizerIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes, typename FKLDebugImGuiFeaturesIterator::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesIterator(_FeatureData, _FeaturesPool)
    , mFeaturesIndexes(_FeaturesIndexes)
{
}

inline bool FKLDebugImGuiFeatureVisualizerIterator::IsValid() const
{
    return mFeaturesIndexes.IsValidIndex(static_cast<int32>(mIndex));
}

inline FKLDebugImGuiFeatureVisualizerEntry& FKLDebugImGuiFeatureVisualizerIterator::GetEntryDataMutable()
{
    return mFeaturesIndexes[mIndex];
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFeatureVisualizerIterator::GetFeatureDataIndexChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex FeatureDataIndex = mFeaturesIndexes[mIndex].GetFeatureIndex();
    return FeatureDataIndex;
}
