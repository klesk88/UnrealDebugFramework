// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugFeatureInterfaceBase;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureManagerEntryBase : public FNoncopyable
{
public:
    explicit FKLDebugFeatureManagerEntryBase(const SIZE_T _ClassSize);
    virtual ~FKLDebugFeatureManagerEntryBase() = default;

    void AddNextEntry(FKLDebugFeatureManagerEntryBase& _NextEntry);
    [[nodiscard]] FKLDebugFeatureManagerEntryBase* GetNextEntry() const;

    [[nodiscard]] virtual EImGuiInterfaceType GetInterfaceType() const = 0;
    [[nodiscard]] virtual IKLDebugFeatureInterfaceBase& AllocateInPlace(void* _PoolStartAddress) const = 0;
    [[nodiscard]] virtual SIZE_T GetSize() const = 0;

private:
    FKLDebugFeatureManagerEntryBase* mNext = nullptr;
};

inline void FKLDebugFeatureManagerEntryBase::AddNextEntry(FKLDebugFeatureManagerEntryBase& _NextEntry)
{
    mNext = &_NextEntry;
}

inline FKLDebugFeatureManagerEntryBase* FKLDebugFeatureManagerEntryBase::GetNextEntry() const
{
    return mNext;
}
