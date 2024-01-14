// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Interface/KLDebugImGuiFeatureInterfaceInternalMacros.h"
#include "Feature/Interface/KLDebugImGuiFeatureInterfaceMacros.h"
#include "Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

// c++
#include <type_traits>

class FKLDebugImGuiFeatureCanvasInput;
class FKLDebugImGuiFeatureContextInput;
class IKLDebugImGuiFilterInterface;
class IKLDebugUserNetworkingFeatureInterfaceBase;
class IKLDebugUserNetworkingFeatureSelectableInterface;
class IKLDebugUserNetworkingFeatureUniqueInterface;
class UObject;
class UWorld;

#define DERIVED_KL_DEBUG_FEATURE_CLASS(ItemType, ParentItemType)                                                                                                \
private:                                                                                                                                                        \
    friend class IKLDebugImGuiFeatureInterface_Selectable;                                                                                                      \
    friend class IKLDebugImGuiFeatureInterface_Unique;                                                                                                          \
                                                                                                                                                                \
public:                                                                                                                                                         \
    UE_NODISCARD inline static const FName& StaticItemType()                                                                                                    \
    {                                                                                                                                                           \
        static const FName Interface(TEXT(#ItemType));                                                                                                          \
        return Interface;                                                                                                                                       \
    }                                                                                                                                                           \
                                                                                                                                                                \
    template <typename FeatureType>                                                                                                                             \
    UE_NODISCARD static bool constexpr IsSameFeatureTypeCheck()                                                                                                 \
    {                                                                                                                                                           \
        return std::is_same_v<ItemType, FeatureType>;                                                                                                           \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD inline constexpr static bool IsDerivedFromParent()                                                                                             \
    {                                                                                                                                                           \
        return TIsDerivedFrom<ItemType, ParentItemType>::IsDerived;                                                                                             \
    }                                                                                                                                                           \
                                                                                                                                                                \
    template <typename InterfaceClass>                                                                                                                          \
    UE_NODISCARD constexpr InterfaceClass* TryCastToInterfaceMutable()                                                                                          \
    {                                                                                                                                                           \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                                                                                      \
        {                                                                                                                                                       \
            return static_cast<InterfaceClass*>(this);                                                                                                          \
        }                                                                                                                                                       \
                                                                                                                                                                \
        return nullptr;                                                                                                                                         \
    }                                                                                                                                                           \
                                                                                                                                                                \
    template <typename InterfaceClass>                                                                                                                          \
    UE_NODISCARD constexpr const InterfaceClass* TryCastToInterface() const                                                                                     \
    {                                                                                                                                                           \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)                                                                                      \
        {                                                                                                                                                       \
            return static_cast<const InterfaceClass*>(this);                                                                                                    \
        }                                                                                                                                                       \
                                                                                                                                                                \
        return nullptr;                                                                                                                                         \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD const IKLDebugUserNetworkingFeatureInterfaceBase* TryGetNetworkInterface() const override                                                      \
    {                                                                                                                                                           \
        if (const IKLDebugUserNetworkingFeatureInterfaceBase* NetworkInterface = TryCastToInterface<IKLDebugUserNetworkingFeatureInterfaceBase>())              \
        {                                                                                                                                                       \
            return NetworkInterface;                                                                                                                            \
        }                                                                                                                                                       \
                                                                                                                                                                \
        return ParentItemType::TryGetNetworkInterface();                                                                                                        \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD IKLDebugUserNetworkingFeatureSelectableInterface* TryGetNetworkSelectableInterfaceMutable() override                                           \
    {                                                                                                                                                           \
        if (IKLDebugUserNetworkingFeatureSelectableInterface* NetworkInterface = TryCastToInterfaceMutable<IKLDebugUserNetworkingFeatureSelectableInterface>()) \
        {                                                                                                                                                       \
            return NetworkInterface;                                                                                                                            \
        }                                                                                                                                                       \
                                                                                                                                                                \
        return ParentItemType::TryGetNetworkSelectableInterfaceMutable();                                                                                       \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD const IKLDebugUserNetworkingFeatureSelectableInterface* TryGetNetworkSelectableInterface() const override                                      \
    {                                                                                                                                                           \
        if (const IKLDebugUserNetworkingFeatureSelectableInterface* NetworkInterface = TryCastToInterface<IKLDebugUserNetworkingFeatureSelectableInterface>())  \
        {                                                                                                                                                       \
            return NetworkInterface;                                                                                                                            \
        }                                                                                                                                                       \
                                                                                                                                                                \
        return ParentItemType::TryGetNetworkSelectableInterface();                                                                                              \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD IKLDebugUserNetworkingFeatureUniqueInterface* TryGetNetworkUniqueInterfaceMutable() override                                                   \
    {                                                                                                                                                           \
        if (IKLDebugUserNetworkingFeatureUniqueInterface* NetworkInterface = TryCastToInterfaceMutable<IKLDebugUserNetworkingFeatureUniqueInterface>())         \
        {                                                                                                                                                       \
            return NetworkInterface;                                                                                                                            \
        }                                                                                                                                                       \
                                                                                                                                                                \
        return ParentItemType::TryGetNetworkUniqueInterfaceMutable();                                                                                           \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD const IKLDebugUserNetworkingFeatureUniqueInterface* TryGetNetworkUniqueInterface() const override                                              \
    {                                                                                                                                                           \
        if (const IKLDebugUserNetworkingFeatureUniqueInterface* NetworkInterface = TryCastToInterface<IKLDebugUserNetworkingFeatureUniqueInterface>())          \
        {                                                                                                                                                       \
            return NetworkInterface;                                                                                                                            \
        }                                                                                                                                                       \
                                                                                                                                                                \
        return ParentItemType::TryGetNetworkUniqueInterface();                                                                                                  \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD inline bool RequireCanvasUpdate() const override                                                                                               \
    {                                                                                                                                                           \
        return RequireCanvasUpdateInternal<ItemType>();                                                                                                         \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD inline bool RequireSceneProxy() const override                                                                                                 \
    {                                                                                                                                                           \
        return RequireSceneProxyInternal<ItemType>();                                                                                                           \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD inline bool RequireTick() const override                                                                                                       \
    {                                                                                                                                                           \
        return RequireTickInternal<ItemType>();                                                                                                                 \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD inline bool RequireDrawImGui() const override                                                                                                  \
    {                                                                                                                                                           \
        return RequireDrawImGuiInternal<ItemType>();                                                                                                            \
    }                                                                                                                                                           \
                                                                                                                                                                \
    UE_NODISCARD inline bool RequireRender() const override                                                                                                     \
    {                                                                                                                                                           \
        return RequireRenderInternal<ItemType>();                                                                                                               \
    }                                                                                                                                                           \
                                                                                                                                                                \
protected:                                                                                                                                                      \
    UE_NODISCARD inline bool IsDerivedInternal(const FName& _ItemTypeName) const override                                                                       \
    {                                                                                                                                                           \
        return StaticItemType() == _ItemTypeName || ParentItemType::IsDerivedInternal(_ItemTypeName);                                                           \
    }                                                                                                                                                           \
                                                                                                                                                                \
private:    // finish macro that allow us to have a basic RTTI system

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
class IKLDebugImGuiFeatureInterfaceBase
{
public:
    virtual ~IKLDebugImGuiFeatureInterfaceBase();

    // get path that will be displayed inside the ImGui tree
    KLDEBUGIMGUIUSER_API UE_NODISCARD virtual const FName& GetImGuiPath() const = 0;

    // this is called once at game startup from the engine module to initialize the class
    KLDEBUGIMGUIUSER_API virtual void Initialize();

    // use the DERIVED_KL_DEBUG_FEATURE_CLASS macro for these
    UE_NODISCARD virtual const IKLDebugUserNetworkingFeatureInterfaceBase* TryGetNetworkInterface() const;
    UE_NODISCARD virtual IKLDebugUserNetworkingFeatureSelectableInterface* TryGetNetworkSelectableInterfaceMutable();
    UE_NODISCARD virtual const IKLDebugUserNetworkingFeatureSelectableInterface* TryGetNetworkSelectableInterface() const;
    UE_NODISCARD virtual IKLDebugUserNetworkingFeatureUniqueInterface* TryGetNetworkUniqueInterfaceMutable();
    UE_NODISCARD virtual const IKLDebugUserNetworkingFeatureUniqueInterface* TryGetNetworkUniqueInterface() const;

    UE_NODISCARD virtual bool RequireCanvasUpdate() const;
    UE_NODISCARD virtual bool RequireSceneProxy() const;
    UE_NODISCARD virtual bool RequireTick() const;
    UE_NODISCARD virtual bool RequireDrawImGui() const;
    UE_NODISCARD virtual bool RequireRender() const;
    // DERIVED_KL_DEBUG_FEATURE_CLASS

    template <typename FeatureType>
    UE_NODISCARD bool IsDerivedFrom() const;

    UE_NODISCARD const FName& GetFeatureNameID() const;

    UE_NODISCARD bool RequiresAnyUpdate() const;

protected:
    KLDEBUGIMGUIUSER_API UE_NODISCARD virtual const FString& GetWindowName() const = 0;

    // use the DERIVED_KL_DEBUG_FEATURE_CLASS macro for these
    UE_NODISCARD virtual bool IsDerivedInternal(const FName& _ItemTypeName) const;
    UE_NODISCARD static const FName& StaticItemType();
    // DERIVED_KL_DEBUG_FEATURE_CLASS
};

inline void IKLDebugImGuiFeatureInterfaceBase::Initialize()
{
}

template <typename FeatureType>
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

inline const IKLDebugUserNetworkingFeatureInterfaceBase* IKLDebugImGuiFeatureInterfaceBase::TryGetNetworkInterface() const
{
    return nullptr;
}

inline IKLDebugUserNetworkingFeatureSelectableInterface* IKLDebugImGuiFeatureInterfaceBase::TryGetNetworkSelectableInterfaceMutable()
{
    return nullptr;
}

inline const IKLDebugUserNetworkingFeatureSelectableInterface* IKLDebugImGuiFeatureInterfaceBase::TryGetNetworkSelectableInterface() const
{
    return nullptr;
}

inline IKLDebugUserNetworkingFeatureUniqueInterface* IKLDebugImGuiFeatureInterfaceBase::TryGetNetworkUniqueInterfaceMutable()
{
    return nullptr;
}

inline const IKLDebugUserNetworkingFeatureUniqueInterface* IKLDebugImGuiFeatureInterfaceBase::TryGetNetworkUniqueInterface() const
{
    return nullptr;
}

inline const FName& IKLDebugImGuiFeatureInterfaceBase::GetFeatureNameID() const
{
    return GetImGuiPath();
}

inline bool IKLDebugImGuiFeatureInterfaceBase::RequireCanvasUpdate() const
{
    return false;
}

inline bool IKLDebugImGuiFeatureInterfaceBase::RequireSceneProxy() const
{
    return false;
}

inline bool IKLDebugImGuiFeatureInterfaceBase::RequireTick() const
{
    return false;
}

inline bool IKLDebugImGuiFeatureInterfaceBase::RequireDrawImGui() const
{
    return false;
}

inline bool IKLDebugImGuiFeatureInterfaceBase::RequireRender() const
{
    return false;
}

inline bool IKLDebugImGuiFeatureInterfaceBase::RequiresAnyUpdate() const
{
    return RequireCanvasUpdate() || RequireSceneProxy() || RequireTick() || RequireDrawImGui() || RequireRender();
}