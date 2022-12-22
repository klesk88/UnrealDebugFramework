#pragma once

#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryDefines.h"

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
    explicit FKLDebugImGuiFeatureManagerEntryBase(const size_t _ClassSize, const EFeatureEntryType _EntryType);
    virtual ~FKLDebugImGuiFeatureManagerEntryBase() = default;

    void AddNextEntry(FKLDebugImGuiFeatureManagerEntryBase& _NextEntry);
    UE_NODISCARD FKLDebugImGuiFeatureManagerEntryBase* GetNextEntry() const;

    UE_NODISCARD virtual IKLDebugImGuiFeatureInterface& AllocateInPlace(void* _PoolStartAddress) const = 0;
    UE_NODISCARD virtual size_t GetSize() const = 0;
    UE_NODISCARD EFeatureEntryType GetEntryType() const;

private:
    FKLDebugImGuiFeatureManagerEntryBase* mNext = nullptr;
    EFeatureEntryType                     mEntryType = EFeatureEntryType::Count;
};

inline void FKLDebugImGuiFeatureManagerEntryBase::AddNextEntry(FKLDebugImGuiFeatureManagerEntryBase& _NextEntry)
{
    mNext = &_NextEntry;
}

inline FKLDebugImGuiFeatureManagerEntryBase* FKLDebugImGuiFeatureManagerEntryBase::GetNextEntry() const
{
    return mNext;
}

inline EFeatureEntryType FKLDebugImGuiFeatureManagerEntryBase::GetEntryType() const
{
    return mEntryType;
}