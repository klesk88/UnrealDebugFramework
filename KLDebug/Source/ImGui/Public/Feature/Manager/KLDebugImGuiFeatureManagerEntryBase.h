#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugImGuiFeatureInterface;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureManagerEntryBase : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureManagerEntryBase(const size_t _ClassSize);
    virtual ~FKLDebugImGuiFeatureManagerEntryBase() = default;

    void AddNextEntry(FKLDebugImGuiFeatureManagerEntryBase& _NextEntry);
    UE_NODISCARD FKLDebugImGuiFeatureManagerEntryBase* GetNextEntry() const;

    UE_NODISCARD virtual IKLDebugImGuiFeatureInterface& AllocateInPlace(void* _PoolStartAddress) const = 0;
    UE_NODISCARD virtual size_t GetSize() const = 0;

private:
    FKLDebugImGuiFeatureManagerEntryBase* mNext = nullptr;
};

inline void FKLDebugImGuiFeatureManagerEntryBase::AddNextEntry(FKLDebugImGuiFeatureManagerEntryBase& _NextEntry)
{
    mNext = &_NextEntry;
}

inline FKLDebugImGuiFeatureManagerEntryBase* FKLDebugImGuiFeatureManagerEntryBase::GetNextEntry() const
{
    return mNext;
}
