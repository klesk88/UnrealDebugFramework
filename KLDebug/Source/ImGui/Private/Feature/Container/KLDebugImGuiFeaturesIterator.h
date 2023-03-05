#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/KLDebugFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUI_API FKLDebugImGuiFeaturesIterator final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeaturesIterator(const TArray<KL::Debug::Features::Types::FeatureIndex>& _FeaturesOffset, TArray<uint8>& _FeaturesPool);
    explicit operator bool() const;
    void     operator++();

    template<typename FeatureInterfaceType>
    UE_NODISCARD const FeatureInterfaceType& GetFeatureInterfaceCasted() const;
    template<typename FeatureInterfaceType>
    UE_NODISCARD FeatureInterfaceType& GetFeatureInterfaceCastedMutable() const;

    UE_NODISCARD KL::Debug::Features::Types::FeatureIndex GetFeatureOffset() const;

private:
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase& GetFeatureMutable() const;

private:
    // offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    // This array is sorted based on the game play tags used
    const TArray<KL::Debug::Features::Types::FeatureIndex>& mFeaturesOffset;
    // pool of features. This is a byte array and all features are allocated inside of it
    // in this way they are all packed in memory close together
    TArray<uint8>& mFeaturesPool;

    uint32 mIndex = 0;
};

inline FKLDebugImGuiFeaturesIterator::operator bool() const
{
    return mIndex < static_cast<uint32>(mFeaturesOffset.Num());
}

inline void FKLDebugImGuiFeaturesIterator::operator++()
{
    ++mIndex;
}

inline KL::Debug::Features::Types::FeatureIndex FKLDebugImGuiFeaturesIterator::GetFeatureOffset() const
{
    return mFeaturesOffset[mIndex];
}

template<typename FeatureInterfaceType>
inline FeatureInterfaceType& FKLDebugImGuiFeaturesIterator::GetFeatureInterfaceCastedMutable() const
{
    checkf(GetFeatureMutable().IsDerivedFrom<FeatureInterfaceType>(), TEXT("casting to wrong type"));
    return reinterpret_cast<FeatureInterfaceType&>(GetFeatureMutable());
}

template<typename FeatureInterfaceType>
inline const FeatureInterfaceType& FKLDebugImGuiFeaturesIterator::GetFeatureInterfaceCasted() const
{
    return GetFeatureInterfaceCastedMutable<FeatureInterfaceType>();
}

inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIterator::GetFeatureMutable() const
{
    const KL::Debug::Features::Types::FeatureIndex Offset = mFeaturesOffset[mIndex];
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[Offset]);
}
