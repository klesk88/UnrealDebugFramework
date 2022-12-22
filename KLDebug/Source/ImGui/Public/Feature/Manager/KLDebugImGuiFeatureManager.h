#pragma once

#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryDefines.h"

// engine
#include "Containers/StaticArray.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureManagerEntryBase;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureManager final : public FNoncopyable
{
private:
    //based on FModuleManager
    friend struct TOptional<FKLDebugImGuiFeatureManager>;

public:
    static FKLDebugImGuiFeatureManager& Get();

    void                                               AddEntry(const size_t _ClassSize, FKLDebugImGuiFeatureManagerEntryBase& _NewEntry);
    UE_NODISCARD FKLDebugImGuiFeatureManagerEntryBase* GetStartEntry() const;
    UE_NODISCARD uint32                                GetEntryCount(const EFeatureEntryType _FeatureType) const;
    UE_NODISCARD size_t                                GetTotalSizeRequired() const;

private:
    FKLDebugImGuiFeatureManager() = default;

private:
    FKLDebugImGuiFeatureManagerEntryBase* mHead = nullptr;
    FKLDebugImGuiFeatureManagerEntryBase* mTail = nullptr;
    TStaticArray<uint32, static_cast<uint32>(EFeatureEntryType::Count)> mFeaturesCount;
    size_t                          mTotalBytesCount = 0;
};

FORCEINLINE FKLDebugImGuiFeatureManager& FKLDebugImGuiFeatureManager::Get()
{
    // based on FModuleManager
    static TOptional<FKLDebugImGuiFeatureManager> Instance(InPlace);
    return Instance.GetValue();
}

inline FKLDebugImGuiFeatureManagerEntryBase* FKLDebugImGuiFeatureManager::GetStartEntry() const
{
    return mHead;
}

inline uint32 FKLDebugImGuiFeatureManager::GetEntryCount(const EFeatureEntryType _FeatureType) const
{
    checkf(_FeatureType != EFeatureEntryType::Count, TEXT("Invalid data pass"));
    return mFeaturesCount[static_cast<uint32>(_FeatureType)];
}

inline size_t FKLDebugImGuiFeatureManager::GetTotalSizeRequired() const
{
    return mTotalBytesCount;
}