// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "Templates/UnrealTypeTraits.h"

template <typename InterfaceType, typename EntryType>
class TKLDebugUtilsStaticMemoryEntryBase : public FNoncopyable
{
public:
    virtual ~TKLDebugUtilsStaticMemoryEntryBase() = default;

    [[nodiscard]] virtual InterfaceType& AllocateInPlace(void* _PoolStartAddress) const = 0;
    [[nodiscard]] virtual SIZE_T GetSize() const = 0;

    void AddNextEntry(EntryType& _NextEntry);
    [[nodiscard]] EntryType* GetNextEntry() const;

private:
    EntryType* mNext = nullptr;
};

template <typename InterfaceType, typename EntryType>
inline void TKLDebugUtilsStaticMemoryEntryBase<InterfaceType, EntryType>::AddNextEntry(EntryType& _NextEntry)
{
    mNext = &_NextEntry;
}

template <typename InterfaceType, typename EntryType>
inline EntryType* TKLDebugUtilsStaticMemoryEntryBase<InterfaceType, EntryType>::GetNextEntry() const
{
    return mNext;
}
