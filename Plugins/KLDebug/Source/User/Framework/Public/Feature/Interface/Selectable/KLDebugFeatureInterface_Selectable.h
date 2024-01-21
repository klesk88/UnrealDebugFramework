// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"

// engine
#include "Containers/Array.h"
#include "DebugRenderSceneProxy.h"
#include "Templates/UniquePtr.h"

class FKLDebugContextGetterInput_Selectable;
class FKLDebugFeatureContextInput_Selectable;
class FKLDebugFeatureDrawCanvasInput_Selectable;
class FKLDebugFeatureImGuiInput_Selectable;
class FKLDebugFeatureRenderInput_Selectable;
class FKLDebugFeatureSceneProxyInput_Selectable;
class FKLDebugFeatureTickInput_Selectable;
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
 * class FClassDebug final : public IKLDebugFeatureInterface_Selectable
 * {
 *      DERIVED_KL_DEBUG_FEATURE_CLASS(FClassDebug, IKLDebugFeatureInterface_Selectable)
 *      ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_WINDOW(FClassDebug)
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugFeatureInterface_Selectable : public IKLDebugFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugFeatureInterface_Selectable, IKLDebugFeatureInterfaceBase)

public:
    virtual void GetFilterPath(TArray<FName>& _OutFilters) const = 0;

    // This is called each time an entity which supports this feature is selected and start to make use of this feature
    // This can be used to setup the owner object that this feature will operate on
    virtual void OnFeatureSelected(UObject& _OwnerObject) const;
    // This is called for each entity which has spawn this feature
    // This can be used to clean the owner object that this feature was operating on
    virtual void OnFeatureUnselected(UObject& _OwnerObject) const;

    UE_NODISCARD virtual TUniquePtr<IKLDebugContextInterface> GetContext(const FKLDebugContextGetterInput_Selectable& _Input) const;

    virtual void Tick(FKLDebugFeatureTickInput_Selectable& _Input);

    // method that is called if RequireCanvasUpdate returns true which allow the user to draw things on the game viewport canvas
    virtual void DrawOnCanvas(FKLDebugFeatureDrawCanvasInput_Selectable& _Input) const;

    UE_NODISCARD virtual TUniquePtr<FDebugRenderSceneProxy> CreateDebugSceneProxy(FKLDebugFeatureSceneProxyInput_Selectable& _Input) const;

    virtual void DrawImGui(FKLDebugFeatureImGuiInput_Selectable& _Input);
    virtual void Render(const FKLDebugFeatureRenderInput_Selectable& _Input) const;

    static UE_NODISCARD constexpr EImGuiInterfaceType GetInterfaceType();

protected:
    virtual void DrawImGuiChild(FKLDebugFeatureImGuiInput_Selectable& _Input);

    template <typename... FilterType>
    void GetFilterPathHelper(TArray<FName>& _OutFilters) const;

private:
    template <typename... FilterType>
    void Dummy(FilterType&&...) const;

    INTERNAL_KL_DEBUG_FEATURE_COMMON_FUNCTIONS(IKLDebugFeatureInterface_Selectable)
};

template <typename... FilterType>
inline void IKLDebugFeatureInterface_Selectable::Dummy(FilterType&&...) const
{
}

template <typename... FilterType>
inline void IKLDebugFeatureInterface_Selectable::GetFilterPathHelper(TArray<FName>& _OutFilters) const
{
    Dummy((_OutFilters.Emplace(FilterType::StaticGetFilterID()))...);

    const int32 Count = _OutFilters.Num();
    for (int32 i = 0; i < Count / 2; ++i)
    {
        _OutFilters.SwapMemory(i, Count - 1 - i);
    }
}

inline void IKLDebugFeatureInterface_Selectable::OnFeatureSelected([[maybe_unused]] UObject& _OwnerObject) const
{
}

inline void IKLDebugFeatureInterface_Selectable::OnFeatureUnselected([[maybe_unused]] UObject& _OwnerObject) const
{
}

inline void IKLDebugFeatureInterface_Selectable::Tick(FKLDebugFeatureTickInput_Selectable& _Input)
{
}

inline void IKLDebugFeatureInterface_Selectable::DrawOnCanvas([[maybe_unused]] FKLDebugFeatureDrawCanvasInput_Selectable& _Input) const
{
}

inline TUniquePtr<FDebugRenderSceneProxy> IKLDebugFeatureInterface_Selectable::CreateDebugSceneProxy([[maybe_unused]] FKLDebugFeatureSceneProxyInput_Selectable& _Input) const
{
    return nullptr;
}

inline void IKLDebugFeatureInterface_Selectable::Render([[maybe_unused]] const FKLDebugFeatureRenderInput_Selectable& _Input) const
{
}

inline constexpr EImGuiInterfaceType IKLDebugFeatureInterface_Selectable::GetInterfaceType()
{
    return EImGuiInterfaceType::SELECTABLE;
}

inline void IKLDebugFeatureInterface_Selectable::DrawImGuiChild([[maybe_unused]] FKLDebugFeatureImGuiInput_Selectable& _Input)
{
}

inline TUniquePtr<IKLDebugContextInterface> IKLDebugFeatureInterface_Selectable::GetContext([[maybe_unused]] const FKLDebugContextGetterInput_Selectable& _Input) const
{
    return nullptr;
}