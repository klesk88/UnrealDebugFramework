// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/KLDebugFeatureInterfaceMacros.h"
#include "Internal/Context/User/KLDebugContextUserInterface.h"
#include "Internal/Feature/Interface/KLDebugFeatureInterfaceInternalMacros.h"
#include "Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"
#include "Internal/Networking/KLDebugNetworkingGetterInterface.h"
#include "Internal/RTTI/KLDebugRTTIInterface.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

// c++
#include <type_traits>

class FKLDebugFeatureCanvasInput;
class IKLDebugFilterInterface;
class UObject;
class UWorld;

#define DERIVED_KL_DEBUG_FEATURE_CLASS(ItemType, ParentItemType)            \
    KL_DEBUG_RTTI(ItemType, ParentItemType)                                 \
    KL_DEBUG_NETWORK_GETTER_INTERFACE(ItemType)                             \
                                                                            \
private:                                                                    \
    friend class IKLDebugFeatureInterface_Selectable;                       \
    friend class IKLDebugFeatureInterface_Unique;                           \
                                                                            \
public:                                                                     \
    template <typename FeatureType>                                         \
    UE_NODISCARD static bool constexpr IsSameFeatureTypeCheck()             \
    {                                                                       \
        return std::is_same_v<ItemType, FeatureType>;                       \
    }                                                                       \
                                                                            \
    template <typename InterfaceClass>                                      \
    UE_NODISCARD constexpr InterfaceClass* TryCastToInterfaceMutable()      \
    {                                                                       \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)  \
        {                                                                   \
            return static_cast<InterfaceClass*>(this);                      \
        }                                                                   \
                                                                            \
        return nullptr;                                                     \
    }                                                                       \
                                                                            \
    template <typename InterfaceClass>                                      \
    UE_NODISCARD constexpr const InterfaceClass* TryCastToInterface() const \
    {                                                                       \
        if constexpr (TIsDerivedFrom<ItemType, InterfaceClass>::IsDerived)  \
        {                                                                   \
            return static_cast<const InterfaceClass*>(this);                \
        }                                                                   \
                                                                            \
        return nullptr;                                                     \
    }                                                                       \
                                                                            \
    UE_NODISCARD inline bool RequireCanvasUpdate() const override           \
    {                                                                       \
        return RequireCanvasUpdateInternal<ItemType>();                     \
    }                                                                       \
                                                                            \
    UE_NODISCARD inline bool RequireSceneProxy() const override             \
    {                                                                       \
        return RequireSceneProxyInternal<ItemType>();                       \
    }                                                                       \
                                                                            \
    UE_NODISCARD inline bool RequireTick() const override                   \
    {                                                                       \
        return RequireTickInternal<ItemType>();                             \
    }                                                                       \
                                                                            \
    UE_NODISCARD inline bool RequireDrawImGui() const override              \
    {                                                                       \
        return RequireDrawImGuiInternal<ItemType>();                        \
    }                                                                       \
                                                                            \
    UE_NODISCARD inline bool RequireRender() const override                 \
    {                                                                       \
        return RequireRenderInternal<ItemType>();                           \
    }                                                                       \
                                                                            \
private:    // finish macro that allow us to have a basic RTTI system

/*
 * Base class for any debug window to extend.
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugFeatureInterface
 * {
 *   ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_WINDOW(FClassDebug)
 */
class IKLDebugFeatureInterfaceBase : public IKLDebugRTTIInterface, public IKLDebugNetworkingGetterInterface
{
public:
    virtual ~IKLDebugFeatureInterfaceBase();

    // get path that will be displayed inside the ImGui tree
    KLDEBUGUSERFRAMEWORK_API UE_NODISCARD virtual const FName& GetImGuiPath() const = 0;

    // this is called once at game startup from the engine module to initialize the class
    KLDEBUGUSERFRAMEWORK_API virtual void Initialize();

    // use the DERIVED_KL_DEBUG_FEATURE_CLASS macro for these
    UE_NODISCARD virtual bool RequireCanvasUpdate() const;
    UE_NODISCARD virtual bool RequireSceneProxy() const;
    UE_NODISCARD virtual bool RequireTick() const;
    UE_NODISCARD virtual bool RequireDrawImGui() const;
    UE_NODISCARD virtual bool RequireRender() const;
    // DERIVED_KL_DEBUG_FEATURE_CLASS

    UE_NODISCARD const FName& GetFeatureNameID() const;

    UE_NODISCARD bool RequiresAnyUpdate() const;

protected:
    KLDEBUGUSERFRAMEWORK_API UE_NODISCARD virtual const FString& GetWindowName() const = 0;
};

inline void IKLDebugFeatureInterfaceBase::Initialize()
{
}

inline const FName& IKLDebugFeatureInterfaceBase::GetFeatureNameID() const
{
    return GetImGuiPath();
}

inline bool IKLDebugFeatureInterfaceBase::RequireCanvasUpdate() const
{
    return false;
}

inline bool IKLDebugFeatureInterfaceBase::RequireSceneProxy() const
{
    return false;
}

inline bool IKLDebugFeatureInterfaceBase::RequireTick() const
{
    return false;
}

inline bool IKLDebugFeatureInterfaceBase::RequireDrawImGui() const
{
    return false;
}

inline bool IKLDebugFeatureInterfaceBase::RequireRender() const
{
    return false;
}

inline bool IKLDebugFeatureInterfaceBase::RequiresAnyUpdate() const
{
    return RequireCanvasUpdate() || RequireSceneProxy() || RequireTick() || RequireDrawImGui() || RequireRender();
}