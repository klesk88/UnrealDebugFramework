// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIteratorBase.h"
#include "Feature/Container/KLDebugImGuiFeatureData.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeaturesConstIterator : public FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesConstIterator, true>
{
private:
    template <class Child, bool IsConst>
    friend class FKLDebugImGuiFeaturesIteratorBase;

public:
    explicit FKLDebugImGuiFeaturesConstIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesConstIterator, true>::PoolType& _FeaturesPool);
    virtual ~FKLDebugImGuiFeaturesConstIterator() = default;

    explicit operator bool() const;

protected:
    UE_NODISCARD virtual bool IsValid() const;
    UE_NODISCARD virtual KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndexChild() const;
};

inline FKLDebugImGuiFeaturesConstIterator::FKLDebugImGuiFeaturesConstIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesConstIterator, true>::PoolType& _FeaturesPool)
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

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFeaturesConstIterator::GetFeatureDataIndexChild() const
{
    return mIndex;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeaturesIterator : public FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIterator, false>
{
private:
    template <class Child, bool IsConst>
    friend class FKLDebugImGuiFeaturesIteratorBase;

public:
    explicit FKLDebugImGuiFeaturesIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIterator, false>::PoolType& _FeaturesPool);
    virtual ~FKLDebugImGuiFeaturesIterator() = default;

    explicit operator bool() const;

protected:
    UE_NODISCARD virtual bool IsValid() const;
    UE_NODISCARD virtual KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndexChild() const;
};

inline FKLDebugImGuiFeaturesIterator::FKLDebugImGuiFeaturesIterator(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, typename FKLDebugImGuiFeaturesIteratorBase<FKLDebugImGuiFeaturesIterator, false>::PoolType& _FeaturesPool)
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

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFeaturesIterator::GetFeatureDataIndexChild() const
{
    return mIndex;
}