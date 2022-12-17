#pragma once

// engine
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

    void                                                AddEntry(const size_t _ClassSize, FKLDebugImGuiFeatureManagerEntryBase& _NewEntry);
    UE_NODISCARD FKLDebugImGuiFeatureManagerEntryBase* GetStartEntry() const;
    UE_NODISCARD int32                                 GetEntryCount() const;
    UE_NODISCARD size_t                                 GetTotalSizeRequired() const;

private:
    FKLDebugImGuiFeatureManager() = default;

private:
    FKLDebugImGuiFeatureManagerEntryBase* mHead = nullptr;
    FKLDebugImGuiFeatureManagerEntryBase* mTail = nullptr;
    int32                           mCount = 0;
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

inline int32 FKLDebugImGuiFeatureManager::GetEntryCount() const
{
    return mCount;
}

inline size_t FKLDebugImGuiFeatureManager::GetTotalSizeRequired() const
{
    return mTotalBytesCount;
}