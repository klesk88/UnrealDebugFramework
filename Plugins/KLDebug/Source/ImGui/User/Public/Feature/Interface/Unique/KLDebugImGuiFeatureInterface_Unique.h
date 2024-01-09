// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

// engine
#include "Templates/UniquePtr.h"

class FDebugRenderSceneProxy;
class FKLDebugFeatureDrawCanvasInput_Unique;
class FKLDebugFeatureSceneProxyInput_Unique;
class FKLDebugFeatureTickInput_Unique;
class FKLDebugImGuiFeatureContextInput_Unique;
class FKLDebugImGuiFeatureImGuiInput_Unique;
class FKLDebugImGuiFeatureRenderInput_Unique;
class UWorld;

/*
 * This class can be used for data which has a unique representation, meaning there is only one instance of it, during the lifetime of the UWorld.
 * For example this could be Unreal Subsystems, singletons, etc. Because of this there is only at most one instance of this feature alive per world
 * (remember tough that in a client/server environment you have multiple worlds valid at the same time). To have data per world in that case create a context
 *
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugImGuiFeatureInterface_Unique
 * {
 *     DERIVED_KL_DEBUG_FEATURE_CLASS(FClassDebug, IKLDebugImGuiFeatureInterface_Unique)
 *     ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_WINDOW(FClassDebug)
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeatureInterface_Unique : public IKLDebugImGuiFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_Unique, IKLDebugImGuiFeatureInterfaceBase)

public:
    // should return true if the world supports this kind of feature
    UE_NODISCARD virtual bool DoesSupportWorld(const UWorld& _World) const = 0;

    // this is called once per world which supports this feature after the world has called its BeginPlay. Only one world at the time for a certain client can be valid
    // so this can be treated as if there is only one feature alive (contrary to IKLDebugImGuiFeatureInterface_Selectable), so you can perform some initialization here for your feature.
    virtual void OnFeatureSelected(const UWorld& _World);
    // this is called when the world supporting this feature has been deinitialize. Only one world at the time for a certain client can be valid
    // so this can be treated as if there is only one feature alive (contrary to IKLDebugImGuiFeatureInterface_Selectable), so you can perform some deinitialization here for your feature.
    virtual void OnFeatureUnselected(const UWorld& _World);

    // this can be used in a network environment where, in editor, we can have multiple valid worlds at the same time (one per client + server in case of a dedicated server environment)
    UE_NODISCARD virtual TUniquePtr<FKLDebugImGuiFeatureContext_Base> GetFeatureContext(const FKLDebugImGuiFeatureContextInput_Unique& _Input) const;

    // method that is called if RequireCanvasUpdate returns true which allow the user to draw things on the game viewport canvas
    virtual void DrawOnCanvas(FKLDebugFeatureDrawCanvasInput_Unique& _Input) const;

    //
    UE_NODISCARD virtual TUniquePtr<FDebugRenderSceneProxy> CreateDebugSceneProxy(FKLDebugFeatureSceneProxyInput_Unique& _Input);

    virtual void Tick(FKLDebugFeatureTickInput_Unique& _Input);

    virtual void DrawImGui(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input);
    virtual void Render(const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const;

    static UE_NODISCARD constexpr EImGuiInterfaceType GetInterfaceType();

protected:
    virtual void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Context);

    INTERNAL_KL_DEBUG_FEATURE_COMMON_FUNCTIONS(IKLDebugImGuiFeatureInterface_Unique)
};

inline void IKLDebugImGuiFeatureInterface_Unique::OnFeatureSelected([[maybe_unused]] const UWorld& _World)
{
}

inline void IKLDebugImGuiFeatureInterface_Unique::OnFeatureUnselected([[maybe_unused]] const UWorld& _World)
{
}

inline TUniquePtr<FKLDebugImGuiFeatureContext_Base> IKLDebugImGuiFeatureInterface_Unique::GetFeatureContext([[maybe_unused]] const FKLDebugImGuiFeatureContextInput_Unique& _Input) const
{
    return nullptr;
}

inline void IKLDebugImGuiFeatureInterface_Unique::DrawOnCanvas([[maybe_unused]] FKLDebugFeatureDrawCanvasInput_Unique& _Input) const
{
}

inline TUniquePtr<FDebugRenderSceneProxy> IKLDebugImGuiFeatureInterface_Unique::CreateDebugSceneProxy([[maybe_unused]] FKLDebugFeatureSceneProxyInput_Unique& _Input)
{
    return nullptr;
}

inline void IKLDebugImGuiFeatureInterface_Unique::Tick(FKLDebugFeatureTickInput_Unique& _Input)
{
}

inline constexpr EImGuiInterfaceType IKLDebugImGuiFeatureInterface_Unique::GetInterfaceType()
{
    return EImGuiInterfaceType::UNIQUE;
}

inline void IKLDebugImGuiFeatureInterface_Unique::Render([[maybe_unused]] const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const
{
}

inline void IKLDebugImGuiFeatureInterface_Unique::DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Context)
{
}