#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIteratorBase.h"
#include "Feature/Container/KLDebugImGuiFeatureData.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"

template<bool IsConst>
class FKLDebugImGuiFeaturesIteratorCommon final : public FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIteratorCommon, IsConst>
{
private:
    template<template<bool IsConst> class Child, bool IsConst>
    friend class FKLDebugImGuiFeaturesIteratorBase;

public:
    explicit FKLDebugImGuiFeaturesIteratorCommon(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIteratorCommon, IsConst>::PoolType& _FeaturesPool);
    explicit operator bool() const;

private:
    template<bool Enabled = IsConst, typename TEnableIf<!Enabled, bool>::Type = true>
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase&       GetFeatureMutableChild() const;
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const;
};

template<bool IsConst>
FKLDebugImGuiFeaturesIteratorCommon<IsConst>::FKLDebugImGuiFeaturesIteratorCommon(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIteratorCommon, IsConst>::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesIteratorBase(_FeatureData, _FeaturesPool)
{
}

template<bool IsConst>
inline FKLDebugImGuiFeaturesIteratorCommon<IsConst>::operator bool() const
{
    return mIndex < static_cast<uint32>(mFeatureData.Num());
}

template<bool IsConst>
template<bool Enabled, typename TEnableIf<!Enabled, bool>::Type>
inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIteratorCommon<IsConst>::GetFeatureMutableChild() const
{
    const FKLDebugImGuiFeatureData&                        FeatureData   = mFeatureData[mIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

template<bool IsConst>
const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIteratorCommon<IsConst>::GetFeatureChild() const
{
    const FKLDebugImGuiFeatureData&                        FeatureData   = mFeatureData[mIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

//////////////////////////////////////////////////////////////////

using FKLDebugImGuiFeaturesIterator      = FKLDebugImGuiFeaturesIteratorCommon<false>;
using FKLDebugImGuiFeaturesConstIterator = FKLDebugImGuiFeaturesIteratorCommon<true>;