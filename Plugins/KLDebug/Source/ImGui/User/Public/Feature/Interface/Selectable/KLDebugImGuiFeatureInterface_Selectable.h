// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureImGuiInput_Selectable.h"
#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureRenderInput_Selectable.h"

// engine
#include "Containers/Array.h"
#include "DebugRenderSceneProxy.h"
#include "Templates/UniquePtr.h"

class FKLDebugFeatureDrawCanvasInput_Selectable;
class FKLDebugFeatureSceneProxyInput_Selectable;
class FKLDebugFeatureTickInput_Selectable;
class FKLDebugImGuiFeatureContextInput_Selectable;
class UObject;

/*
 * This class can be used for represent data which can be selected in some form. For example this can be represented by an AActor
 * that is picked in the game viewport.
 * The class is shared between all the entities which support the feature. To allow for data on a per entity basis, please create a context,
 * by overriding GetFeatureContext.
 *
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugImGuiFeatureInterface_Selectable
 * {
 *      DERIVED_KL_DEBUG_FEATURE_CLASS(FClassDebug, IKLDebugImGuiFeatureInterface_Selectable)
 *      ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_WINDOW(FClassDebug)
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeatureInterface_Selectable : public IKLDebugImGuiFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_Selectable, IKLDebugImGuiFeatureInterfaceBase)

public:
    virtual void GetFilterPath(TArray<FName>& _OutFilters) const = 0;

    // This is called each time an entity which supports this feature is selected and start to make use of this feature
    // This can be used to setup the owner object that this feature will operate on
    virtual void OnFeatureSelected(UObject& _OwnerObject) const;
    // This is called for each entity which has spawn this feature
    // This can be used to clean the owner object that this feature was operating on
    virtual void OnFeatureUnselected(UObject& _OwnerObject) const;

    virtual void Tick(FKLDebugFeatureTickInput_Selectable& _Input);

    // allow the feature to have a context per instance. This means that, for example, if we have 2 different Pawns
    // using the same feature, each of them will have their own context class independent of each other
    //(while the feature class itself is shared)
    UE_NODISCARD virtual TUniquePtr<FKLDebugImGuiFeatureContext_Base> GetFeatureContext(const FKLDebugImGuiFeatureContextInput_Selectable& _Input) const;

    // method that is called if RequireCanvasUpdate returns true which allow the user to draw things on the game viewport canvas
    virtual void DrawOnCanvas(FKLDebugFeatureDrawCanvasInput_Selectable& _Input) const;

    UE_NODISCARD virtual TUniquePtr<FDebugRenderSceneProxy> CreateDebugSceneProxy(FKLDebugFeatureSceneProxyInput_Selectable& _Input) const;

    virtual void DrawImGui(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input);
    virtual void Render(const FKLDebugImGuiFeatureRenderInput_Selectable& _Input) const;

    static UE_NODISCARD constexpr EImGuiInterfaceType GetInterfaceType();

protected:
    virtual void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input);

    template <typename... FilterType>
    void GetFilterPathHelper(TArray<FName>& _OutFilters) const;

private:
    template <typename... FilterType>
    void Dummy(FilterType&&...) const;

    INTERNAL_KL_DEBUG_FEATURE_COMMON_FUNCTIONS(IKLDebugImGuiFeatureInterface_Selectable)
};

template <typename... FilterType>
inline void IKLDebugImGuiFeatureInterface_Selectable::GetFilterPathHelper(TArray<FName>& _OutFilters) const
{
    Dummy((_OutFilters.Emplace(FilterType::StaticGetFilterID()))...);

    const int32 Count = _OutFilters.Num();
    for (int32 i = 0; i < Count / 2; ++i)
    {
        _OutFilters.SwapMemory(i, Count - 1 - i);
    }
}

inline void IKLDebugImGuiFeatureInterface_Selectable::OnFeatureSelected([[maybe_unused]] UObject& _OwnerObject) const
{
}

inline void IKLDebugImGuiFeatureInterface_Selectable::OnFeatureUnselected([[maybe_unused]] UObject& _OwnerObject) const
{
}

inline TUniquePtr<FKLDebugImGuiFeatureContext_Base> IKLDebugImGuiFeatureInterface_Selectable::GetFeatureContext([[maybe_unused]] const FKLDebugImGuiFeatureContextInput_Selectable& _Input) const
{
    return nullptr;
}

inline void IKLDebugImGuiFeatureInterface_Selectable::Tick(FKLDebugFeatureTickInput_Selectable& _Input)
{
}

inline void IKLDebugImGuiFeatureInterface_Selectable::DrawOnCanvas([[maybe_unused]] FKLDebugFeatureDrawCanvasInput_Selectable& _Input) const
{
}

inline TUniquePtr<FDebugRenderSceneProxy> IKLDebugImGuiFeatureInterface_Selectable::CreateDebugSceneProxy([[maybe_unused]] FKLDebugFeatureSceneProxyInput_Selectable& _Input) const
{
    return nullptr;
}

template <typename... FilterType>
inline void IKLDebugImGuiFeatureInterface_Selectable::Dummy(FilterType&&...) const
{
}

inline void IKLDebugImGuiFeatureInterface_Selectable::Render(const FKLDebugImGuiFeatureRenderInput_Selectable& _Input) const
{
}

inline constexpr EImGuiInterfaceType IKLDebugImGuiFeatureInterface_Selectable::GetInterfaceType()
{
    return EImGuiInterfaceType::SELECTABLE;
}

inline void IKLDebugImGuiFeatureInterface_Selectable::DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input)
{
}