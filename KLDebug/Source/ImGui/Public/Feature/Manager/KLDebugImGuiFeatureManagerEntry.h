#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

// engine
#include "GameplayTagContainer.h"

template<typename FeatureInterfaceType>
class TKLDebugImGuiFeatureManagerEntry final : public FKLDebugImGuiFeatureManagerEntryBase
{
public:
    explicit TKLDebugImGuiFeatureManagerEntry(const FName& _NameToCheck);

    // FKLDebugWindowManagerEntryBase
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD size_t                             GetSize() const final;
    // FKLDebugWindowManagerEntryBase
};

template<typename FeatureInterfaceType>
TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::TKLDebugImGuiFeatureManagerEntry(const FName& _NameToCheck)
    : FKLDebugImGuiFeatureManagerEntryBase(sizeof(FeatureInterfaceType))
{
    static_assert(TIsDerivedFrom<FeatureInterfaceType, IKLDebugImGuiFeatureInterfaceBase>::IsDerived, "Class passed must derived from IKLDebugImGuiFeatureInterfaceBase");
    static_assert(FeatureInterfaceType::IsDerivedFromParent(), TEXT("feature has the DERIVED_KL_DEBUG_FEATURE_CLASS macro setup wrong has it doesnt derived from the passed parent"));
    checkf(FeatureInterfaceType::StaticItemType() == _NameToCheck, TEXT("feature [%s] must define macro DERIVED_KL_DEBUG_FEATURE_CLASS in its .h file"), *_NameToCheck.ToString());
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
