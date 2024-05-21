// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"
#include "Internal/Feature/Manager/KLDebugFeatureManagerEntryBase.h"
#include "Internal/Networking/KLDebugNetworkingBaseInterface.h"

// engine
#include "Templates/UnrealTypeTraits.h"

template <typename FeatureInterfaceType>
class TKLDebugFeatureManagerEntry final : public FKLDebugFeatureManagerEntryBase
{
public:
    explicit TKLDebugFeatureManagerEntry(const FName& _NameToCheck);

    // FKLDebugFeatureManagerEntryBase
    [[nodiscard]] EImGuiInterfaceType GetInterfaceType() const final;
    [[nodiscard]] IKLDebugFeatureInterfaceBase& AllocateInPlace(void* _PoolStartAddress) const final;
    [[nodiscard]] SIZE_T GetSize() const final;
    // FKLDebugFeatureManagerEntryBase
};

template <typename FeatureInterfaceType>
TKLDebugFeatureManagerEntry<FeatureInterfaceType>::TKLDebugFeatureManagerEntry(const FName& _NameToCheck)
    : FKLDebugFeatureManagerEntryBase(sizeof(FeatureInterfaceType))
{
    static_assert(TIsDerivedFrom<FeatureInterfaceType, IKLDebugFeatureInterfaceBase>::IsDerived, "Class passed must derived from IKLDebugFeatureInterfaceBase");
    static_assert(FeatureInterfaceType::template IsSameFeatureTypeCheck<FeatureInterfaceType>(), "feature has a wrong class type passed to the macro KL_DERIVED_DEBUG_FEATURE_CLASS");
    static_assert(FeatureInterfaceType::IsDerivedFromParent(), TEXT("feature has the KL_DERIVED_DEBUG_FEATURE_CLASS macro setup wrong has it doesnt derived from the passed parent"));
    checkf(FeatureInterfaceType::StaticItemType() == _NameToCheck, TEXT("feature [%s] must define macro KL_DERIVED_DEBUG_FEATURE_CLASS in its .h file"), *_NameToCheck.ToString());

    if constexpr (TIsDerivedFrom<FeatureInterfaceType, IKLDebugNetworkingBaseInterface>::IsDerived)
    {
        FeatureInterfaceType::template NetworkStaticChecks<FeatureInterfaceType>();
    }
}

template <typename FeatureInterfaceType>
IKLDebugFeatureInterfaceBase& TKLDebugFeatureManagerEntry<FeatureInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugFeatureInterfaceBase*>(new (_PoolStartAddress) FeatureInterfaceType());
}

template <typename FeatureInterfaceType>
SIZE_T TKLDebugFeatureManagerEntry<FeatureInterfaceType>::GetSize() const
{
    return sizeof(FeatureInterfaceType);
}

template <typename FeatureInterfaceType>
inline EImGuiInterfaceType TKLDebugFeatureManagerEntry<FeatureInterfaceType>::GetInterfaceType() const
{
    return FeatureInterfaceType::GetInterfaceType();
}
