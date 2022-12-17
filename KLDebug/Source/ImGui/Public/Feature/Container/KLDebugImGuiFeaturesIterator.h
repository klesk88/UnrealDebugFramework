#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugImGuiFeatureInterface;

class KLDEBUGIMGUI_API FKLDebugImGuiFeaturesIterator final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeaturesIterator(const TArray<uint32>& _FeaturesOffset, TArray<uint8>& _FeaturesPool);
    explicit operator bool() const;
    void     operator++();

    UE_NODISCARD IKLDebugImGuiFeatureInterface& GetFeatureMutable() const;

private:
    //offset between features so that we can retrieve them correctly from the pool. Each entry is the
    //start address index of a new feature inside mFeaturesPool.
    //This array is sorted based on the game play tags used
    const TArray<uint32>& mFeaturesOffset;
    //pool of features. This is a byte array and all features are allocated inside of it
    //in this way they are all packed in memory close together
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
