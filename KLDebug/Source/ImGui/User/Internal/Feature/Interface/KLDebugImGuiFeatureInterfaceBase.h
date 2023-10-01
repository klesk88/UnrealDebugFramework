#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Interface/KLDebugImGuiFeatureInterfaceMacros.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiFeatureContextInput;
class IKLDebugImGuiFilterInterface;
class IKLDebugImGuiFeature_NetworkingInterface;
class UWorld;

#define DERIVED_KL_DEBUG_FEATURE_CLASS(ItemType, ParentItemType)                                      \
public:                                                                                               \
    UE_NODISCARD inline static const FName& StaticItemType()                                          \
    {                                                                                                 \
        static const FName Interface(TEXT(#ItemType));                                                \
        return Interface;                                                                             \
    }                                                                                                 \
                                                                                                      \
    UE_NODISCARD inline constexpr static bool IsDerivedFromParent()                                   \
    {                                                                                                 \
        return TIsDerivedFrom<ItemType, ParentItemType>::IsDerived;                                   \
    }                                                                                                 \
                                                                                                      \
    template<typename InterfaceClass>                                                                 \
    UE_NODISCARD constexpr InterfaceClass* TryCastToInterfaceMutable()                                \
    {                                                                                                 \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                            \
        {                                                                                             \
            return static_cast<InterfaceClass*>(this);                                                \
        }                                                                                             \
                                                                                                      \
        return nullptr;                                                                               \
    }                                                                                                 \
                                                                                                      \
    template<typename InterfaceClass>                                                                 \
    UE_NODISCARD constexpr const InterfaceClass* TryCastToInterface() const                           \
    {                                                                                                 \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                            \
        {                                                                                             \
            return static_cast<const InterfaceClass*>(this);                                          \
        }                                                                                             \
                                                                                                      \
        return nullptr;                                                                               \
    }                                                                                                 \
                                                                                                      \
    UE_NODISCARD IKLDebugImGuiFeature_NetworkingInterface* TryGetNetworkInterfaceMutable() override   \
    {                                                                                                 \
        if(IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = TryCastToInterfaceMutable<IKLDebugImGuiFeature_NetworkingInterface>()) \
        {                                                                                               \
            return NetworkInterface;                                                                    \
        }                                                                                               \
                                                                                                        \
        return ParentItemType::TryGetNetworkInterfaceMutable();                                         \
    }                                                                                                   \
                                                                                                        \
    UE_NODISCARD const IKLDebugImGuiFeature_NetworkingInterface* TryGetNetworkInterface() const override \
    {                                                                                                   \
        if (const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = TryCastToInterface<IKLDebugImGuiFeature_NetworkingInterface>())\
        {                                                                                             \
            return NetworkInterface;                                                                  \
        }                                                                                             \
                                                                                                      \
        return ParentItemType::TryGetNetworkInterface();                                              \
    }                                                                                                 \
                                                                                                      \
protected:                                                                                            \
    UE_NODISCARD inline bool IsDerivedInternal(const FName& _ItemTypeName) const override             \
    {                                                                                                 \
        return StaticItemType() == _ItemTypeName || ParentItemType::IsDerivedInternal(_ItemTypeName); \
    }                                                                                                 \
                                                                                                      \
private:  // finish macro that allow us to have a basic RTTI system

/*
 * Base class for any debug window to extend.
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugImGuiFeatureInterface
 * {
 *   ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_WINDOW(FClassDebug)
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeatureInterfaceBase
{
public:
    virtual ~IKLDebugImGuiFeatureInterfaceBase();

    // get path that will be displayed inside the ImGui tree
    UE_NODISCARD virtual const FName& GetImGuiPath() const = 0;

    // this is called once at game startup from the engine module to initialize the class
    virtual void Initialize();
    //allow the feature to have a context per instance. This means that, for example, if we have 2 different Pawns
    //using the same feature, each of them will have their own context class independent of each other
    //(while the feature class itself is shared)
    UE_NODISCARD virtual TUniquePtr<FKLDebugImGuiFeatureContext_Base> GetFeatureContext(const FKLDebugImGuiFeatureContextInput& _Input) const;

    // use the DERIVED_KL_DEBUG_FEATURE_CLASS macro for these
    UE_NODISCARD virtual IKLDebugImGuiFeature_NetworkingInterface* TryGetNetworkInterfaceMutable();
    UE_NODISCARD virtual const IKLDebugImGuiFeature_NetworkingInterface* TryGetNetworkInterface() const;
    //DERIVED_KL_DEBUG_FEATURE_CLASS

    template<typename FeatureType>
    UE_NODISCARD bool IsDerivedFrom() const;

    UE_NODISCARD const FName& GetFeatureNameID() const;

protected:
    UE_NODISCARD virtual const FString& GetWindowName() const = 0;

    // use the DERIVED_KL_DEBUG_FEATURE_CLASS macro for these
    UE_NODISCARD virtual bool IsDerivedInternal(const FName& _ItemTypeName) const;
    UE_NODISCARD static const FName& StaticItemType();
    //DERIVED_KL_DEBUG_FEATURE_CLASS
};

inline void IKLDebugImGuiFeatureInterfaceBase::Initialize()
{
}

template<typename FeatureType>
inline bool IKLDebugImGuiFeatureInterfaceBase::IsDerivedFrom() const
{
    return IsDerivedInternal(FeatureType::StaticItemType());
}

inline bool IKLDebugImGuiFeatureInterfaceBase::IsDerivedInternal(const FName& _ItemTypeName) const
{
    return _ItemTypeName == StaticItemType();
}

inline const FName& IKLDebugImGuiFeatureInterfaceBase::StaticItemType()
{
    static const FName Interface(TEXT("ImGuiFeatureInterfaceBase"));
    return Interface;
}

inline TUniquePtr<FKLDebugImGuiFeatureContext_Base> IKLDebugImGuiFeatureInterfaceBase::GetFeatureContext([[maybe_unused]] const FKLDebugImGuiFeatureContextInput& _Input) const
{
    return nullptr;
}

inline IKLDebugImGuiFeature_NetworkingInterface* IKLDebugImGuiFeatureInterfaceBase::TryGetNetworkInterfaceMutable()
{
    return nullptr;
}

inline const IKLDebugImGuiFeature_NetworkingInterface* IKLDebugImGuiFeatureInterfaceBase::TryGetNetworkInterface() const
{
    return nullptr;
}

inline const FName& IKLDebugImGuiFeatureInterfaceBase::GetFeatureNameID() const
{
    return GetImGuiPath();
}