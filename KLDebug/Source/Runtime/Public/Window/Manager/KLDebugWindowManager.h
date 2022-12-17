#pragma once

// engine
#include "CoreMinimal.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugWindowManagerEntryBase;

class KLDEBUGRUNTIME_API FKLDebugWindowManager final : public FNoncopyable
{
private:
    //based on FModuleManager
    friend struct TOptional<FKLDebugWindowManager>;

public:
    static FKLDebugWindowManager& Get();

    void                                           AddEntry(FKLDebugWindowManagerEntryBase& _NewEntry);
    UE_NODISCARD FKLDebugWindowManagerEntryBase* GetStartEntry() const;
    UE_NODISCARD int32                                 GetEntryCount() const;

private:
    FKLDebugWindowManager() = default;

private:
    FKLDebugWindowManagerEntryBase* mHead = nullptr;
    FKLDebugWindowManagerEntryBase* mTail = nullptr;
    int32                           mCount = 0;
};

inline FKLDebugWindowManager& FKLDebugWindowManager::Get()
{
    // based on FModuleManager
    static TOptional<FKLDebugWindowManager> Instance(InPlace);
    return Instance.GetValue();
}

inline FKLDebugWindowManagerEntryBase* FKLDebugWindowManager::GetStartEntry() const
{
    return mHead;
}

inline int32 FKLDebugWindowManager::GetEntryCount() const
{
    return mCount;
}