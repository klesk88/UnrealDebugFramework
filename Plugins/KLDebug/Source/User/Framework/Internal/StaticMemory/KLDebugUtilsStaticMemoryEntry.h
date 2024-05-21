// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "Templates/UnrealTypeTraits.h"

template <typename InterfaceType, typename EntryType>
class TKLDebugImGuiStaticMemoryEntryBase : public FNoncopyable
{
public:
    virtual ~TKLDebugImGuiStaticMemoryEntryBase() = default;

    void AddNextEntry(EntryType& _NextEntry);
    [[nodiscard]] EntryType* GetNextEntry() const;

    [[nodiscard]] virtual InterfaceType& AllocateInPlace(void* _PoolStartAddress) const = 0;
    [[nodiscard]] virtual SIZE_T GetSize() const = 0;

private:
    EntryType* mNext = nullptr;
};

template <typename InterfaceType, typename EntryType>
inline void TKLDebugImGuiStaticMemoryEntryBase<InterfaceType, EntryType>::AddNextEntry(EntryType& _NextEntry)
{
    mNext = &_NextEntry;
}

template <typename InterfaceType, typename EntryType>
inline EntryType* TKLDebugImGuiStaticMemoryEntryBase<InterfaceType, EntryType>::GetNextEntry() const
{
    return mNext;
}
