#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"

class KLDEBUGIMGUI_API FKLDebugImGuiSubsetFeaturesConstIterator final : public FKLDebugImGuiFeaturesConstIterator
{
public:
    explicit FKLDebugImGuiSubsetFeaturesConstIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesConstIterator::PoolType& _FeaturesPool);

private:
    // FKLDebugImGuiFeaturesConstIterator
    UE_NODISCARD bool                                     IsValid() const final;
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const final;
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

inline const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiSubsetFeaturesConstIterator::GetFeatureChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex  FeatureDataIndex = mFeaturesIndexes[mIndex];
    const FKLDebugImGuiFeatureData&                        FeatureData      = mFeatureData[FeatureDataIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset    = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

//////////////////////////////////////////////////////////////////

class KLDEBUGIMGUI_API FKLDebugImGuiSubsetFeaturesIterator final : public FKLDebugImGuiFeaturesIterator
{
public:
    explicit FKLDebugImGuiSubsetFeaturesIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIterator::PoolType& _FeaturesPool);

private:
    // FKLDebugImGuiFeaturesIterator
    UE_NODISCARD bool                                     IsValid() const final;
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const final;
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase&       GetFeatureMutableChild() const final;
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

inline const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiSubsetFeaturesIterator::GetFeatureChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex  FeatureDataIndex = mFeaturesIndexes[mIndex];
    const FKLDebugImGuiFeatureData&                        FeatureData      = mFeatureData[FeatureDataIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset    = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiSubsetFeaturesIterator::GetFeatureMutableChild() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex  FeatureDataIndex = mFeaturesIndexes[mIndex];
    const FKLDebugImGuiFeatureData&                        FeatureData      = mFeatureData[FeatureDataIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset    = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}