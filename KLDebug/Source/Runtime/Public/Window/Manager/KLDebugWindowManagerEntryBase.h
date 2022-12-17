#pragma once

// engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugWindow;

class KLDEBUGRUNTIME_API FKLDebugWindowManagerEntryBase : public FNoncopyable
{
public:
    FKLDebugWindowManagerEntryBase();
    virtual ~FKLDebugWindowManagerEntryBase() = default;

    void AddNextEntry(FKLDebugWindowManagerEntryBase& _NextEntry);
    UE_NODISCARD FKLDebugWindowManagerEntryBase* GetNextEntry() const;
    UE_NODISCARD virtual IKLDebugWindow& GetInterface() = 0;

private:
    FKLDebugWindowManagerEntryBase* mNext = nullptr;
};

inline void FKLDebugWindowManagerEntryBase::AddNextEntry(FKLDebugWindowManagerEntryBase& _NextEntry)
{
    mNext = &_NextEntry;
}

inline FKLDebugWindowManagerEntryBase* FKLDebugWindowManagerEntryBase::GetNextEntry() const
{
    return mNext;
}
