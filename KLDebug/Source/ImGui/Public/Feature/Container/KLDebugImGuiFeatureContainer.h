#pragma once

#include "Feature/Container/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryDefines.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureManager;
class IKLDebugImGuiFeatureInterface;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureContainer final : public FNoncopyable
{
public:
    void Initialize();
    void Shutdown();

    FKLDebugImGuiFeaturesIterator GetFeatureIteratorMutable(const EFeatureEntryType _FeatureType);

private:
    UE_NODISCARD IKLDebugImGuiFeatureInterface&       GetFeatureMutable(const uint32 _Offset);
    UE_NODISCARD const IKLDebugImGuiFeatureInterface& GetFeature(const uint32 _Offset) const;

    void GatherFeatures();
    void InitializeOffsets(const FKLDebugImGuiFeatureManager& _FeatureManager);
    UE_NODISCARD TArray<uint32>& GetFeatureOffsetsMutable(const EFeatureEntryType _FeatureType);
    void SortFeatures();
    void SortFeatures(TArray<uint32>& _FeaturesOffset) const;
    void DestroyFeatures();

private:
    //offset between features so that we can retrieve them correctly from the pool. Each entry is the
    //start address index of a new feature inside mFeaturesPool.
    //This array is sorted based on the game play tags used
    TArray<TArray<uint32>, TInlineAllocator<static_cast<uint32>(EFeatureEntryType::Count)>> mFeaturesOffset;
    //pool of features. This is a byte array and all features are allocated inside of it
    //in this way they are all packed in memory close together
    TArray<uint8> mFeaturesPool;
};

inline FKLDebugImGuiFeaturesIterator FKLDebugImGuiFeatureContainer::GetFeatureIteratorMutable(const EFeatureEntryType _FeatureType)
{
    checkf(_FeatureType != EFeatureEntryType::Count, TEXT("Invalid data pass"));
    return FKLDebugImGuiFeaturesIterator(mFeaturesOffset[static_cast<uint32>(_FeatureType)], mFeaturesPool);
}

inline TArray<uint32>& FKLDebugImGuiFeatureContainer::GetFeatureOffsetsMutable(const EFeatureEntryType _FeatureType)
{
    checkf(_FeatureType != EFeatureEntryType::Count, TEXT("Invalid data pass"));
    return mFeaturesOffset[static_cast<uint32>(_FeatureType)];
}