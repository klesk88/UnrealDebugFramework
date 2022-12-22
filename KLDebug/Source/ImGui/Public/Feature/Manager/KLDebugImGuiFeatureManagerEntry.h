#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterface.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

//engine
#include "GameplayTagContainer.h"
#include "Templates/UnrealTypeTraits.h"

class IKLDebugImGuiFeatureInterface;

template<typename WindowInterfaceType>
class TKLDebugImGuiFeatureManagerEntry final : public FKLDebugImGuiFeatureManagerEntryBase
{
public:
    explicit TKLDebugImGuiFeatureManagerEntry(const EFeatureEntryType _EntryType);

    //FKLDebugWindowManagerEntryBase
    UE_NODISCARD IKLDebugImGuiFeatureInterface& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD size_t GetSize() const final;
    //FKLDebugWindowManagerEntryBase
};

template<typename WindowInterfaceType>
TKLDebugImGuiFeatureManagerEntry<WindowInterfaceType>::TKLDebugImGuiFeatureManagerEntry(const EFeatureEntryType _EntryType)
    : FKLDebugImGuiFeatureManagerEntryBase(sizeof(WindowInterfaceType), _EntryType)
{
    static_assert(TIsDerivedFrom<WindowInterfaceType, IKLDebugImGuiFeatureInterface>::IsDerived, "Class passed must derived from IKLDebugWindow");
}

template<typename WindowInterfaceType>
IKLDebugImGuiFeatureInterface& TKLDebugImGuiFeatureManagerEntry<WindowInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugImGuiFeatureInterface*>(new (_PoolStartAddress) WindowInterfaceType());
}

template<typename WindowInterfaceType>
size_t TKLDebugImGuiFeatureManagerEntry<WindowInterfaceType>::GetSize() const
{
    return sizeof(WindowInterfaceType);
}
