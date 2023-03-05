#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

// engine
#include "GameplayTagContainer.h"
#include "Templates/UnrealTypeTraits.h"

class IKLDebugImGuiFeatureInterfaceBase;

template<typename FeatureInterfaceType>
class TKLDebugImGuiFeatureManagerEntry final : public FKLDebugImGuiFeatureManagerEntryBase
{
public:
    TKLDebugImGuiFeatureManagerEntry();

    // FKLDebugWindowManagerEntryBase
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD size_t                             GetSize() const final;
    // FKLDebugWindowManagerEntryBase
};

template<typename FeatureInterfaceType>
TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::TKLDebugImGuiFeatureManagerEntry()
    : FKLDebugImGuiFeatureManagerEntryBase(sizeof(FeatureInterfaceType))
{
    static_assert(TIsDerivedFrom<FeatureInterfaceType, IKLDebugImGuiFeatureInterfaceBase>::IsDerived, "Class passed must derived from IKLDebugWindow");
}

template<typename FeatureInterfaceType>
IKLDebugImGuiFeatureInterfaceBase& TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (_PoolStartAddress) FeatureInterfaceType());
}

template<typename FeatureInterfaceType>
size_t TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::GetSize() const
{
    return sizeof(FeatureInterfaceType);
}
