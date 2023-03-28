#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIteratorBase.h"
#include "Feature/Container/KLDebugImGuiFeatureData.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"

class FKLDebugImGuiFeaturesConstIterator : public FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesConstIterator, true>
{
private:
    template<class Child, bool IsConst>
    friend class FKLDebugImGuiFeaturesIteratorBase;

public:
    explicit FKLDebugImGuiFeaturesConstIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesConstIterator, true>::PoolType& _FeaturesPool);
    virtual ~FKLDebugImGuiFeaturesConstIterator() = default;

    explicit operator bool() const;

protected:
    UE_NODISCARD virtual bool                                     IsValid() const;
    UE_NODISCARD virtual const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const;
};

FKLDebugImGuiFeaturesConstIterator::FKLDebugImGuiFeaturesConstIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesConstIterator, true>::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesIteratorBase(_FeatureData, _FeaturesPool)
{
}

inline FKLDebugImGuiFeaturesConstIterator::operator bool() const
{
    return IsValid();
}

inline bool FKLDebugImGuiFeaturesConstIterator::IsValid() const
{
    return mIndex < static_cast<uint32>(mFeatureData.Num());
}

inline const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesConstIterator::GetFeatureChild() const
{
    const FKLDebugImGuiFeatureData&                        FeatureData   = mFeatureData[mIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FKLDebugImGuiFeaturesIterator : public FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIterator, false>
{
private:
    template<class Child, bool IsConst>
    friend class FKLDebugImGuiFeaturesIteratorBase;

public:
    explicit FKLDebugImGuiFeaturesIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIterator, false>::PoolType& _FeaturesPool);
    virtual ~FKLDebugImGuiFeaturesIterator() = default;

    explicit operator bool() const;

protected:
    UE_NODISCARD virtual bool                                     IsValid() const;
    UE_NODISCARD virtual const IKLDebugImGuiFeatureInterfaceBase& GetFeatureChild() const;
    UE_NODISCARD virtual IKLDebugImGuiFeatureInterfaceBase&       GetFeatureMutableChild() const;
};

FKLDebugImGuiFeaturesIterator::FKLDebugImGuiFeaturesIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIterator, false>::PoolType& _FeaturesPool)
    : FKLDebugImGuiFeaturesIteratorBase(_FeatureData, _FeaturesPool)
{
}

inline FKLDebugImGuiFeaturesIterator::operator bool() const
{
    return IsValid();
}

inline bool FKLDebugImGuiFeaturesIterator::IsValid() const
{
    return mIndex < static_cast<uint32>(mFeatureData.Num());
}

inline const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIterator::GetFeatureChild() const
{
    const FKLDebugImGuiFeatureData&                        FeatureData   = mFeatureData[mIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}

inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIterator::GetFeatureMutableChild() const
{
    const FKLDebugImGuiFeatureData&                        FeatureData   = mFeatureData[mIndex];
    const KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = FeatureData.GetFeatureOffset();
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureOffset]);
}
