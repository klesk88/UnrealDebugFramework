#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerConstIterator final : public FKLDebugImGuiFeaturesConstIterator
{
public:
    explicit FKLDebugImGuiFeatureVisualizerConstIterator(const TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesConstIterator::PoolType& _FeaturesPool);

private:
    // FKLDebugImGuiFeaturesConstIterator
    UE_NODISCARD bool                                     IsValid() const final;
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const final;
    // FKLDebugImGuiFeaturesConstIterator

    const TArray<FKLDebugImGuiFeatureVisualizerEntry>& mFeaturesIndexes;
};

inline FKLDebugImGuiFeatureVisualizerConstIterator::FKLDebugImGuiFeatureVisualizerConstIterator(const TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesConstIterator::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesConstIterator(_FeatureData, _FeaturesPool)
    , mFeaturesIndexes(_FeaturesIndexes)
{
}

inline bool FKLDebugImGuiFeatureVisualizerConstIterator::IsValid() const
{
    return mIndex < static_cast<uint32>(mFeaturesIndexes.Num());
}

inline const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeatureVisualizerConstIterator::GetFeatureChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex  FeatureDataIndex = mFeaturesIndexes[mIndex].GetFeatureIndex();
    const FKLDebugImGuiFeatureData&                        FeatureData      = mFeatureData[FeatureDataIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset    = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

//////////////////////////////////////////////////////////////////

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerIterator final : public FKLDebugImGuiFeaturesIterator
{
public:
    explicit FKLDebugImGuiFeatureVisualizerIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes, typename FKLDebugImGuiFeaturesIterator::PoolType& _FeaturesPool);

    UE_NODISCARD FKLDebugImGuiFeatureVisualizerEntry& GetEntryDataMutable();

private:
    // FKLDebugImGuiFeaturesIterator
    UE_NODISCARD bool                                     IsValid() const final;
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const final;
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase&       GetFeatureMutableChild() const final;
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
    return mIndex < static_cast<uint32>(mFeaturesIndexes.Num());
}

inline const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeatureVisualizerIterator::GetFeatureChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex  FeatureDataIndex = mFeaturesIndexes[mIndex].GetFeatureIndex();
    const FKLDebugImGuiFeatureData&                        FeatureData      = mFeatureData[FeatureDataIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset    = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeatureVisualizerIterator::GetFeatureMutableChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex  FeatureDataIndex = mFeaturesIndexes[mIndex].GetFeatureIndex();
    const FKLDebugImGuiFeatureData&                        FeatureData      = mFeatureData[FeatureDataIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset    = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

inline FKLDebugImGuiFeatureVisualizerEntry& FKLDebugImGuiFeatureVisualizerIterator::GetEntryDataMutable()
{
    return mFeaturesIndexes[mIndex];
}