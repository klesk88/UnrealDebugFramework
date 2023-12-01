// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiSubsetFeaturesConstIterator final : public FKLDebugImGuiFeaturesConstIterator
{
public:
    explicit FKLDebugImGuiSubsetFeaturesConstIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesConstIterator::PoolType& _FeaturesPool);

private:
    // FKLDebugImGuiFeaturesConstIterator
    UE_NODISCARD bool IsValid() const final;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndexChild() const final;
    // FKLDebugImGuiFeaturesConstIterator

    const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& mFeaturesIndexes;
};

inline FKLDebugImGuiSubsetFeaturesConstIterator::FKLDebugImGuiSubsetFeaturesConstIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesConstIterator::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesConstIterator(_FeatureData, _FeaturesPool)
    , mFeaturesIndexes(_FeaturesIndexes)
{
}

inline bool FKLDebugImGuiSubsetFeaturesConstIterator::IsValid() const
{
    return mIndex < static_cast<uint32>(mFeaturesIndexes.Num());
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiSubsetFeaturesConstIterator::GetFeatureDataIndexChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex FeatureIndex = mFeaturesIndexes[mIndex];
    return FeatureIndex;
}

//////////////////////////////////////////////////////////////////

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiSubsetFeaturesIterator final : public FKLDebugImGuiFeaturesIterator
{
public:
    explicit FKLDebugImGuiSubsetFeaturesIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIterator::PoolType& _FeaturesPool);

private:
    // FKLDebugImGuiFeaturesIterator
    UE_NODISCARD bool IsValid() const final;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndexChild() const final;
    // FKLDebugImGuiFeaturesIterator

private:
    const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& mFeaturesIndexes;
};

inline FKLDebugImGuiSubsetFeaturesIterator::FKLDebugImGuiSubsetFeaturesIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIterator::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesIterator(_FeatureData, _FeaturesPool)
    , mFeaturesIndexes(_FeaturesIndexes)
{
}

inline bool FKLDebugImGuiSubsetFeaturesIterator::IsValid() const
{
    return mIndex < static_cast<uint32>(mFeaturesIndexes.Num());
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiSubsetFeaturesIterator::GetFeatureDataIndexChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex FeatureIndex = mFeaturesIndexes[mIndex];
    return FeatureIndex;
}