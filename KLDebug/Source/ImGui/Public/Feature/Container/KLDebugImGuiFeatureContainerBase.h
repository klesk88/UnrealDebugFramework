#pragma once

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/KLDebugImGuiFeatureData.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/Iterator/KLDebugImGuiFeatureVisualizerIterator.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Math/NumericLimits.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureManager;
class FKLDebugImGuiFeatureManagerEntryBase;
class FKLDebugImGuiFeatureVisualizerEntry;
class FString;
class IKLDebugImGuiFeatureInterfaceBase;
class UObject;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureContainerBase : public FNoncopyable
{
public:
    virtual ~FKLDebugImGuiFeatureContainerBase() = default;

    UE_NODISCARD virtual bool IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const = 0;

    UE_NODISCARD bool IsValidFeatureIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID) const;
    UE_NODISCARD uint32 GetFeaturesCount() const;
    UE_NODISCARD TOptional<FName> TryGetFeatureNameID(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const;

    void InitGenerateFeatures(const uint32 _Size, const uint32 _EntryCount);
    void AllocateNewEntry(const FKLDebugImGuiFeatureManagerEntryBase& _Entry, const KL::Debug::ImGui::Features::Types::FeatureOffset _OffsetIndex, TArray<FString>& _PathString);
    void FinishGenerateFeatures();

    void GatherFeatures(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const;

    UE_NODISCARD FKLDebugImGuiFeaturesIterator      GetFeaturesIterator();
    UE_NODISCARD FKLDebugImGuiFeaturesConstIterator GetFeaturesConstIterator() const;
    
    UE_NODISCARD FKLDebugImGuiFeatureVisualizerIterator GetFeatureVisualizerIterator(TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes);
    UE_NODISCARD FKLDebugImGuiFeatureVisualizerConstIterator GetFeatureVisualizerConstIterator(const TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes) const;

    UE_NODISCARD FKLDebugImGuiSubsetFeaturesIterator      GetFeaturesSubsetIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes);
    UE_NODISCARD FKLDebugImGuiSubsetFeaturesConstIterator GetFeaturesSubsetConstIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes) const;

    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const;
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase& GetFeatureMutable(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);

protected:
    virtual void GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const = 0;
    virtual void FinishGenerateFeatureChild();

    UE_NODISCARD const TArray<FKLDebugImGuiFeatureData>& GetFeaturesData() const;
    UE_NODISCARD TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& GetFeaturesPoolMutable();
    UE_NODISCARD const TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& GetFeaturesPool() const;

private:
    // offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    TArray<FKLDebugImGuiFeatureData> mFeaturesData;
    // pool of features. This is a byte array and all features are allocated inside of it
    // in this way they are all packed in memory close together
    TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue> mFeaturesPool;
};

inline uint32 FKLDebugImGuiFeatureContainerBase::GetFeaturesCount() const
{
    return static_cast<uint32>(mFeaturesData.Num());
}

inline FKLDebugImGuiFeaturesIterator FKLDebugImGuiFeatureContainerBase::GetFeaturesIterator()
{
    return FKLDebugImGuiFeaturesIterator(mFeaturesData, mFeaturesPool);
}

inline FKLDebugImGuiFeaturesConstIterator FKLDebugImGuiFeatureContainerBase::GetFeaturesConstIterator() const
{
    return FKLDebugImGuiFeaturesConstIterator(mFeaturesData, mFeaturesPool);
}

inline FKLDebugImGuiSubsetFeaturesIterator FKLDebugImGuiFeatureContainerBase::GetFeaturesSubsetIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes)
{
    return FKLDebugImGuiSubsetFeaturesIterator(_FeaturesIndexes, mFeaturesData, mFeaturesPool);
}

inline FKLDebugImGuiSubsetFeaturesConstIterator FKLDebugImGuiFeatureContainerBase::GetFeaturesSubsetConstIterator(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes) const
{
    return FKLDebugImGuiSubsetFeaturesConstIterator(_FeaturesIndexes, mFeaturesData, mFeaturesPool);
}

inline FKLDebugImGuiFeatureVisualizerIterator FKLDebugImGuiFeatureContainerBase::GetFeatureVisualizerIterator(TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes)
{
    return FKLDebugImGuiFeatureVisualizerIterator(mFeaturesData, _FeaturesIndexes, mFeaturesPool);
}

inline FKLDebugImGuiFeatureVisualizerConstIterator FKLDebugImGuiFeatureContainerBase::GetFeatureVisualizerConstIterator(const TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexes) const
{
    return FKLDebugImGuiFeatureVisualizerConstIterator(_FeaturesIndexes, mFeaturesData, mFeaturesPool);
}

inline void FKLDebugImGuiFeatureContainerBase::InitGenerateFeatures(const uint32 _Size, const uint32 _EntryCount)
{
    mFeaturesData.Reserve(_EntryCount);
    mFeaturesPool.AddZeroed(_Size);
}

inline void FKLDebugImGuiFeatureContainerBase::FinishGenerateFeatures()
{
    checkf(mFeaturesData.Num() < TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max(), TEXT("too many features"));

    mFeaturesData.Shrink();
    mFeaturesPool.Shrink();

    FinishGenerateFeatureChild();
}

inline void FKLDebugImGuiFeatureContainerBase::FinishGenerateFeatureChild()
{
}

inline const TArray<FKLDebugImGuiFeatureData>& FKLDebugImGuiFeatureContainerBase::GetFeaturesData() const
{
    return mFeaturesData;
}

inline TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& FKLDebugImGuiFeatureContainerBase::GetFeaturesPoolMutable()
{
    return mFeaturesPool;
}

inline const TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& FKLDebugImGuiFeatureContainerBase::GetFeaturesPool() const
{
    return mFeaturesPool;
}

inline void FKLDebugImGuiFeatureContainerBase::GatherFeatures(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    _OutFeaturesIndexes.Reserve(mFeaturesData.Num());
    GatherFeaturesChild(_Obj, _OutFeaturesIndexes);
    _OutFeaturesIndexes.Shrink();
}