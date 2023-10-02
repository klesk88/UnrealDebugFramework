#pragma once

// engine
#include "Containers/StaticArray.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFilterManagerEntryBase;

template<typename Child, typename EntryType>
class TKLDebugImGuiStaticMemoryManager : public FNoncopyable
{
private:
    // based on FModuleManager
    friend struct TOptional<Child>;

public:
    static Child& Get();

    void                    AddEntry(const size_t _ClassSize, EntryType& _NewEntry);
    UE_NODISCARD EntryType* GetStartEntry() const;
    UE_NODISCARD size_t     GetTotalSizeRequired() const;
    UE_NODISCARD uint32     GetEntryCount() const;

protected:
    TKLDebugImGuiStaticMemoryManager() = default;

private:
    EntryType* mHead             = nullptr;
    EntryType* mTail             = nullptr;
    size_t     mTotalBytesCount  = 0;
    uint32     mEntryCount       = 0;
};

template<typename Child, typename EntryType>
FORCEINLINE Child& TKLDebugImGuiStaticMemoryManager<Child, EntryType>::Get()
{
    // based on FModuleManager
    static TOptional<Child> Instance(InPlace);
    return Instance.GetValue();
}

template<typename Child, typename EntryType>
inline EntryType* TKLDebugImGuiStaticMemoryManager<Child, EntryType>::GetStartEntry() const
{
    return mHead;
}

template<typename Child, typename EntryType>
inline size_t TKLDebugImGuiStaticMemoryManager<Child, EntryType>::GetTotalSizeRequired() const
{
    return mTotalBytesCount;
}

template<typename Child, typename EntryType>
inline uint32 TKLDebugImGuiStaticMemoryManager<Child, EntryType>::GetEntryCount() const
{
    return mEntryCount;
}

template<typename Child, typename EntryType>
void TKLDebugImGuiStaticMemoryManager<Child, EntryType>::AddEntry(const size_t _ClassSize, EntryType& _NewEntry)
{
    mTotalBytesCount += _ClassSize;
    ++mEntryCount;

    if (!mHead)
    {
        mHead = &_NewEntry;
        mTail = &_NewEntry;
        return;
    }

    mTail->AddNextEntry(_NewEntry);
    mTail = &_NewEntry;
}

