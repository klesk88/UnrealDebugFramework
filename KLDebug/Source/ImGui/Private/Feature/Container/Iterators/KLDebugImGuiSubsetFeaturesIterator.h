#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIteratorBase.h"

template<bool IsConst>
class FKLDebugImGuiSubsetFeaturesIteratorCommon final : public FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiSubsetFeaturesIteratorCommon, IsConst>
{
private:
    template<template<bool IsConst> class Child, bool IsConst>
    friend class FKLDebugImGuiFeaturesIteratorBase;

public:
    explicit FKLDebugImGuiSubsetFeaturesIteratorCommon(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiSubsetFeaturesIteratorCommon, IsConst>::PoolType& _FeaturesPool);
    explicit operator bool() const;

private:
    template<bool Enabled = IsConst, typename TEnableIf<!Enabled, bool>::Type = true>
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase&       GetFeatureMutableChild() const;
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const;

    const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& mFeaturesIndexes;
};

template<bool IsConst>
FKLDebugImGuiSubsetFeaturesIteratorCommon<IsConst>::FKLDebugImGuiSubsetFeaturesIteratorCommon(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiSubsetFeaturesIteratorCommon, IsConst>::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesIteratorBase(_FeatureData, _FeaturesPool)
    , mFeaturesIndexes(_FeaturesIndexes)
{
}

template<bool IsConst>
inline FKLDebugImGuiSubsetFeaturesIteratorCommon<IsConst>::operator bool() const
{
    return mIndex < static_cast<uint32>(mFeaturesIndexes.Num());
}

template<bool IsConst>
template<bool Enabled, typename TEnableIf<!Enabled, bool>::Type>
inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiSubsetFeaturesIteratorCommon<IsConst>::GetFeatureMutableChild() const
{
    const FKLDebugImGuiFeatureData&                        FeatureData   = mFeatureData[mIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

template<bool IsConst>
const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiSubsetFeaturesIteratorCommon<IsConst>::GetFeatureChild() const
{
    const FKLDebugImGuiFeatureData&                        FeatureData   = mFeatureData[mIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

//////////////////////////////////////////////////////////////////

using FKLDebugImGuiSubsetFeaturesIterator      = FKLDebugImGuiSubsetFeaturesIteratorCommon<false>;
using FKLDebugImGuiSubsetFeaturesConstIterator = FKLDebugImGuiSubsetFeaturesIteratorCommon<true>;