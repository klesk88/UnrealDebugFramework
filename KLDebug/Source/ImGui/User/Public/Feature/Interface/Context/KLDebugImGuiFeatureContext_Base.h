// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

class IKLDebugImGuiNetworking_ContextInterface;

#define DERIVED_KL_DEBUG_FEATURE_CONTEXT_CLASS(ItemType, ParentItemType)                                                                        \
public:                                                                                                                                         \
    UE_NODISCARD inline static const FName& StaticItemType()                                                                                    \
    {                                                                                                                                           \
        static const FName Interface(TEXT(#ItemType));                                                                                          \
        return Interface;                                                                                                                       \
    }                                                                                                                                           \
                                                                                                                                                \
    template <typename InterfaceClass>                                                                                                          \
    UE_NODISCARD constexpr InterfaceClass* TryCastToInterfaceMutable()                                                                          \
    {                                                                                                                                           \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                                                                      \
        {                                                                                                                                       \
            return static_cast<InterfaceClass*>(this);                                                                                          \
        }                                                                                                                                       \
                                                                                                                                                \
        return nullptr;                                                                                                                         \
    }                                                                                                                                           \
                                                                                                                                                \
    template <typename InterfaceClass>                                                                                                          \
    UE_NODISCARD constexpr const InterfaceClass* TryCastToInterface() const                                                                     \
    {                                                                                                                                           \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                                                                      \
        {                                                                                                                                       \
            return static_cast<const InterfaceClass*>(this);                                                                                    \
        }                                                                                                                                       \
                                                                                                                                                \
        return nullptr;                                                                                                                         \
    }                                                                                                                                           \
                                                                                                                                                \
    UE_NODISCARD IKLDebugImGuiNetworking_ContextInterface* TryGetNetworkInterfaceMutable() override                                             \
    {                                                                                                                                           \
        if (IKLDebugImGuiNetworking_ContextInterface* NetworkInterface = TryCastToInterfaceMutable<IKLDebugImGuiNetworking_ContextInterface>()) \
        {                                                                                                                                       \
            return NetworkInterface;                                                                                                            \
        }                                                                                                                                       \
                                                                                                                                                \
        return ParentItemType::TryGetNetworkInterfaceMutable();                                                                                 \
    }                                                                                                                                           \
                                                                                                                                                \
    UE_NODISCARD const IKLDebugImGuiNetworking_ContextInterface* TryGetNetworkInterface() const override                                        \
    {                                                                                                                                           \
        if (const IKLDebugImGuiNetworking_ContextInterface* NetworkInterface = TryCastToInterface<IKLDebugImGuiNetworking_ContextInterface>())  \
        {                                                                                                                                       \
            return NetworkInterface;                                                                                                            \
        }                                                                                                                                       \
                                                                                                                                                \
        return ParentItemType::TryGetNetworkInterface();                                                                                        \
    }                                                                                                                                           \
                                                                                                                                                \
protected:                                                                                                                                      \
    UE_NODISCARD inline bool IsDerivedInternal(const FName& _ItemTypeName) const override                                                       \
    {                                                                                                                                           \
        return StaticItemType() == _ItemTypeName || ParentItemType::IsDerivedInternal(_ItemTypeName);                                           \
    }                                                                                                                                           \
                                                                                                                                                \
private:    // finish macro that allow us to have a basic RTTI system

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureContext_Base
{
public:
    virtual ~FKLDebugImGuiFeatureContext_Base();

    template <typename FeatureContextType>
    UE_NODISCARD bool IsDerivedFrom() const;

    template <typename FeatureContextType>
    UE_NODISCARD bool IsSameContext() const;

    // use the DERIVED_KL_DEBUG_FEATURE_CONTEXT_CLASS macro for these
    UE_NODISCARD virtual IKLDebugImGuiNetworking_ContextInterface* TryGetNetworkInterfaceMutable();
    UE_NODISCARD virtual const IKLDebugImGuiNetworking_ContextInterface* TryGetNetworkInterface() const;
    // DERIVED_KL_DEBUG_FEATURE_CONTEXT_CLASS

protected:
    // use the DERIVED_KL_DEBUG_FEATURE_CONTEXT_CLASS macro for these
    UE_NODISCARD virtual bool IsDerivedInternal(const FName& _ItemTypeName) const;
    UE_NODISCARD static const FName& StaticItemType();
    //
};

template <typename FeatureContextType>
inline bool FKLDebugImGuiFeatureContext_Base::IsSameContext() const
{
    return FeatureContextType::StaticItemType() == StaticItemType();
}

template <typename FeatureContextType>
inline bool FKLDebugImGuiFeatureContext_Base::IsDerivedFrom() const
{
    return IsDerivedInternal(FeatureContextType::StaticItemType());
}

inline bool FKLDebugImGuiFeatureContext_Base::IsDerivedInternal(const FName& _ItemTypeName) const
{
    return _ItemTypeName == StaticItemType();
}

inline const FName& FKLDebugImGuiFeatureContext_Base::StaticItemType()
{
    static const FName Interface(TEXT("KLDebugImGuiFeatureContext_Base"));
    return Interface;
}

inline IKLDebugImGuiNetworking_ContextInterface* FKLDebugImGuiFeatureContext_Base::TryGetNetworkInterfaceMutable()
{
    return nullptr;
}

inline const IKLDebugImGuiNetworking_ContextInterface* FKLDebugImGuiFeatureContext_Base::TryGetNetworkInterface() const
{
    return nullptr;
}