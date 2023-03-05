#pragma once

#include "Feature/Container/KLDebugImGuiFeaturesIterator.h"
#include "Feature/KLDebugFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureManager;
class FKLDebugImGuiFeatureManagerEntryBase;
class IKLDebugImGuiFeatureInterfaceBase;
class UObject;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureContainerBase : public FNoncopyable
{
public:
    virtual ~FKLDebugImGuiFeatureContainerBase() = default;

    UE_NODISCARD virtual bool IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const = 0;

    void InitGenerateFeatures(const uint32 _Size, const uint32 _EntryCount);
    void AllocateNewEntry(const FKLDebugImGuiFeatureManagerEntryBase& _Entry, const KL::Debug::Features::Types::FeatureIndex _OffsetIndex);
    void FinishGenerateFeatures();

    void                                       GatherFeatures(const UObject& _Obj, TArray<KL::Debug::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const;
    UE_NODISCARD FKLDebugImGuiFeaturesIterator GetFeatureIterator(const TArray<KL::Debug::Features::Types::FeatureIndex>& _FeaturesIndexes);

protected:
    virtual void GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const = 0;
    virtual void FinishGenerateFeatureChild();

    UE_NODISCARD FKLDebugImGuiFeaturesIterator GetFeatureIteratorMutable();
    UE_NODISCARD KL::Debug::Features::Types::FeatureIndex GetFeaturesCount() const;
    UE_NODISCARD const TArray<KL::Debug::Features::Types::FeatureIndex>& GetFeaturesOffset() const;
    UE_NODISCARD const TArray<uint8>& GetFeaturesPool() const;

private:
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase&       GetFeatureMutable(const KL::Debug::Features::Types::FeatureIndex _Offset);
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeature(const KL::Debug::Features::Types::FeatureIndex _Offset) const;

private:
    // offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    // This array is sorted based on the game play tags used
    TArray<KL::Debug::Features::Types::FeatureIndex> mFeaturesOffset;
    // pool of features. This is a byte array and all features are allocated inside of it
    // in this way they are all packed in memory close together
    TArray<uint8> mFeaturesPool;
};

inline FKLDebugImGuiFeaturesIterator FKLDebugImGuiFeatureContainerBase::GetFeatureIteratorMutable()
{
    return GetFeatureIterator(mFeaturesOffset);
}

inline FKLDebugImGuiFeaturesIterator FKLDebugImGuiFeatureContainerBase::GetFeatureIterator(const TArray<KL::Debug::Features::Types::FeatureIndex>& _FeaturesIndexes)
{
    return FKLDebugImGuiFeaturesIterator(_FeaturesIndexes, mFeaturesPool);
}

inline void FKLDebugImGuiFeatureContainerBase::InitGenerateFeatures(const uint32 _Size, const uint32 _EntryCount)
{
    mFeaturesOffset.Reserve(_EntryCount);
    mFeaturesPool.AddZeroed(_Size);
}

inline void FKLDebugImGuiFeatureContainerBase::FinishGenerateFeatures()
{
    mFeaturesOffset.Shrink();
    mFeaturesPool.Shrink();

    FinishGenerateFeatureChild();
}

inline void FKLDebugImGuiFeatureContainerBase::FinishGenerateFeatureChild()
{
}

inline KL::Debug::Features::Types::FeatureIndex FKLDebugImGuiFeatureContainerBase::GetFeaturesCount() const
{
    return mFeaturesOffset.Num();
}

inline const TArray<KL::Debug::Features::Types::FeatureIndex>& FKLDebugImGuiFeatureContainerBase::GetFeaturesOffset() const
{
    return mFeaturesOffset;
}

inline const TArray<uint8>& FKLDebugImGuiFeatureContainerBase::GetFeaturesPool() const
{
    return mFeaturesPool;
}

inline void FKLDebugImGuiFeatureContainerBase::GatherFeatures(const UObject& _Obj, TArray<KL::Debug::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    _OutFeaturesIndexes.Reserve(mFeaturesOffset.Num());
    GatherFeaturesChild(_Obj, _OutFeaturesIndexes);
    _OutFeaturesIndexes.Shrink();
}