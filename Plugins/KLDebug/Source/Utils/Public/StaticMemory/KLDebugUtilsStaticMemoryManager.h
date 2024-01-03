// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

template <typename Child, typename EntryType>
class TKLDebugUtilsStaticMemoryManager : public FNoncopyable
{
private:
    // based on FModuleManager
    friend struct TOptional<Child>;

public:
    UE_NODISCARD static Child& Get();

    void AddEntry(const SIZE_T _ClassSize, EntryType& _NewEntry);
    UE_NODISCARD EntryType* GetStartEntry() const;
    UE_NODISCARD SIZE_T GetTotalSizeRequired() const;
    UE_NODISCARD uint32 GetEntryCount() const;

protected:
    TKLDebugUtilsStaticMemoryManager() = default;

private:
    EntryType* mHead = nullptr;
    EntryType* mTail = nullptr;
    SIZE_T mTotalBytesCount = 0;
    uint32 mEntryCount = 0;
};

template <typename Child, typename EntryType>
FORCEINLINE Child& TKLDebugUtilsStaticMemoryManager<Child, EntryType>::Get()
{
    // based on FModuleManager
    static TOptional<Child> Instance(InPlace);
    return Instance.GetValue();
}

template <typename Child, typename EntryType>
inline EntryType* TKLDebugUtilsStaticMemoryManager<Child, EntryType>::GetStartEntry() const
{
    return mHead;
}

template <typename Child, typename EntryType>
inline SIZE_T TKLDebugUtilsStaticMemoryManager<Child, EntryType>::GetTotalSizeRequired() const
{
    return mTotalBytesCount;
}

template <typename Child, typename EntryType>
inline uint32 TKLDebugUtilsStaticMemoryManager<Child, EntryType>::GetEntryCount() const
{
    return mEntryCount;
}

template <typename Child, typename EntryType>
void TKLDebugUtilsStaticMemoryManager<Child, EntryType>::AddEntry(const SIZE_T _ClassSize, EntryType& _NewEntry)
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
