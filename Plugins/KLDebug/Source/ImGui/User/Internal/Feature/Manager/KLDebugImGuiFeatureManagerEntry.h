// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"
#include "Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"

// engine
#include "Templates/UnrealTypeTraits.h"

template <typename FeatureInterfaceType>
class TKLDebugImGuiFeatureManagerEntry final : public FKLDebugImGuiFeatureManagerEntryBase
{
public:
    explicit TKLDebugImGuiFeatureManagerEntry(const FName& _NameToCheck);

    // FKLDebugImGuiFeatureManagerEntryBase
    UE_NODISCARD EImGuiInterfaceType GetInterfaceType() const final;
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase& AllocateInPlace(void* _PoolStartAddress) const final;
    UE_NODISCARD size_t GetSize() const final;
    // FKLDebugImGuiFeatureManagerEntryBase
};

template <typename FeatureInterfaceType>
TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::TKLDebugImGuiFeatureManagerEntry(const FName& _NameToCheck)
    : FKLDebugImGuiFeatureManagerEntryBase(sizeof(FeatureInterfaceType))
{
    static_assert(TIsDerivedFrom<FeatureInterfaceType, IKLDebugImGuiFeatureInterfaceBase>::IsDerived, "Class passed must derived from IKLDebugImGuiFeatureInterfaceBase");
    static_assert(FeatureInterfaceType::template IsSameFeatureTypeCheck<FeatureInterfaceType>(), "feature has a wrong class type passed to the macro KL_DERIVED_DEBUG_FEATURE_CLASS");
    static_assert(FeatureInterfaceType::IsDerivedFromParent(), TEXT("feature has the KL_DERIVED_DEBUG_FEATURE_CLASS macro setup wrong has it doesnt derived from the passed parent"));
    checkf(FeatureInterfaceType::StaticItemType() == _NameToCheck, TEXT("feature [%s] must define macro KL_DERIVED_DEBUG_FEATURE_CLASS in its .h file"), *_NameToCheck.ToString());

    if constexpr (TIsDerivedFrom<FeatureInterfaceType, IKLDebugImGuiFeature_NetworkingInterface>::IsDerived)
    {
        IKLDebugImGuiFeature_NetworkingInterface::PerformStaticChecks<FeatureInterfaceType>();
    }
}

template <typename FeatureInterfaceType>
IKLDebugImGuiFeatureInterfaceBase& TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::AllocateInPlace(void* _PoolStartAddress) const
{
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (_PoolStartAddress) FeatureInterfaceType());
}

template <typename FeatureInterfaceType>
size_t TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::GetSize() const
{
    return sizeof(FeatureInterfaceType);
}

template <typename FeatureInterfaceType>
inline EImGuiInterfaceType TKLDebugImGuiFeatureManagerEntry<FeatureInterfaceType>::GetInterfaceType() const
{
    return FeatureInterfaceType::GetInterfaceType();
}
