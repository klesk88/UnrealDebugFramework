#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugImGuiFilterInterface;

template<typename InterfaceType, typename EntryType>
class TKLDebugImGuiStaticMemoryEntryBase : public FNoncopyable
{
public:
    virtual ~TKLDebugImGuiStaticMemoryEntryBase() = default;

    void                    AddNextEntry(EntryType& _NextEntry);
    UE_NODISCARD EntryType* GetNextEntry() const;

    UE_NODISCARD virtual InterfaceType& AllocateInPlace(void* _PoolStartAddress) const = 0;
    UE_NODISCARD virtual size_t         GetSize() const                                = 0;

private:
    EntryType* mNext = nullptr;
};

template<typename InterfaceType, typename EntryType>
inline void TKLDebugImGuiStaticMemoryEntryBase<InterfaceType, EntryType>::AddNextEntry(EntryType& _NextEntry)
{
    mNext = &_NextEntry;
}

template<typename InterfaceType, typename EntryType>
inline EntryType* TKLDebugImGuiStaticMemoryEntryBase<InterfaceType, EntryType>::GetNextEntry() const
{
    return mNext;
}
