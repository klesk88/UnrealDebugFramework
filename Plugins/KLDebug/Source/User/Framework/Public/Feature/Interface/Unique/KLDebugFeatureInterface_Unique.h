// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"
#include "Feature/Interface/KLDebugFeatureInterfaceBase.h"

// engine
#include "DebugRenderSceneProxy.h"
#include "Templates/UniquePtr.h"

class FDebugRenderSceneProxy;
class FKLDebugFeatureContextInput_Unique;
class FKLDebugFeatureDrawCanvasInput_Unique;
class FKLDebugFeatureImGuiInput_Unique;
class FKLDebugFeatureRenderInput_Unique;
class FKLDebugFeatureSceneProxyInput_Unique;
class FKLDebugFeatureTickInput_Unique;
class UWorld;

/*
 * This class can be used for data which has a unique representation, meaning there is only one instance of it, during the lifetime of the UWorld.
 * For example this could be Unreal Subsystems, singletons, etc. Because of this there is only at most one instance of this feature alive per world
 * (remember tough that in a client/server environment you have multiple worlds valid at the same time). To have data per world in that case create a context
 *
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_FEATURE() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugFeatureInterface_Unique
 * {
 *     DERIVED_KL_DEBUG_FEATURE_CLASS(FClassDebug, IKLDebugFeatureInterface_Unique)
 *     ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_FEATURE(FClassDebug)
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugFeatureInterface_Unique : public IKLDebugFeatureInterfaceBase, public IKLDebugContextUserInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugFeatureInterface_Unique, IKLDebugFeatureInterfaceBase)

public:
    // should return true if the world supports this kind of feature
    [[nodiscard]] virtual bool DoesSupportWorld(const UWorld& _World) const = 0;

    // this is called once per world which supports this feature after the world has called its BeginPlay. Only one world at the time for a certain client can be valid
    // so this can be treated as if there is only one feature alive (contrary to IKLDebugFeatureInterface_Selectable), so you can perform some initialization here for your feature.
    virtual void OnFeatureSelected(const UWorld& _World);
    // this is called when the world supporting this feature has been deinitialize. Only one world at the time for a certain client can be valid
    // so this can be treated as if there is only one feature alive (contrary to IKLDebugFeatureInterface_Selectable), so you can perform some deinitialization here for your feature.
    virtual void OnFeatureUnselected(const UWorld& _World);

    // method that is called if RequireCanvasUpdate returns true which allow the user to draw things on the game viewport canvas
    virtual void DrawOnCanvas(FKLDebugFeatureDrawCanvasInput_Unique& _Input) const;

    //
    [[nodiscard]] virtual TUniquePtr<FDebugRenderSceneProxy> CreateDebugSceneProxy(FKLDebugFeatureSceneProxyInput_Unique& _Input);

    virtual void Tick(FKLDebugFeatureTickInput_Unique& _Input);

    virtual void DrawImGui(FKLDebugFeatureImGuiInput_Unique& _Input);
    virtual void Render(const FKLDebugFeatureRenderInput_Unique& _Input) const;

    [[nodiscard]] static constexpr EImGuiInterfaceType GetInterfaceType();

protected:
    virtual void DrawImGuiChild(FKLDebugFeatureImGuiInput_Unique& _Context);

    INTERNAL_KL_DEBUG_FEATURE_COMMON_FUNCTIONS(IKLDebugFeatureInterface_Unique)
};

inline void IKLDebugFeatureInterface_Unique::OnFeatureSelected([[maybe_unused]] const UWorld& _World)
{
}

inline void IKLDebugFeatureInterface_Unique::OnFeatureUnselected([[maybe_unused]] const UWorld& _World)
{
}

inline TUniquePtr<FDebugRenderSceneProxy> IKLDebugFeatureInterface_Unique::CreateDebugSceneProxy([[maybe_unused]] FKLDebugFeatureSceneProxyInput_Unique& _Input)
{
    return nullptr;
}

inline void IKLDebugFeatureInterface_Unique::DrawOnCanvas([[maybe_unused]] FKLDebugFeatureDrawCanvasInput_Unique& _Input) const
{
}

inline void IKLDebugFeatureInterface_Unique::Tick([[maybe_unused]] FKLDebugFeatureTickInput_Unique& _Input)
{
}

inline constexpr EImGuiInterfaceType IKLDebugFeatureInterface_Unique::GetInterfaceType()
{
    return EImGuiInterfaceType::UNIQUE;
}

inline void IKLDebugFeatureInterface_Unique::Render([[maybe_unused]] const FKLDebugFeatureRenderInput_Unique& _Input) const
{
}

inline void IKLDebugFeatureInterface_Unique::DrawImGuiChild([[maybe_unused]] FKLDebugFeatureImGuiInput_Unique& _Context)
{
}