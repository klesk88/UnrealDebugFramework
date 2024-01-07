// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

class FKLDebugImGuiFeatureImGuiInput_Unique;
class FKLDebugImGuiFeatureRenderInput_Unique;
class UWorld;

/*
 * This class can be used for data which has a unique representation, meaning there is only one instance of it, during the lifetime of the UWorld.
 * For example this could be Unreal Subsystems, singletons, etc. Because of this there is only at most one instance of this feature alive per world
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

    virtual void DrawImGui(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input);
    virtual void Render(const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const;

    static UE_NODISCARD EImGuiInterfaceType GetInterfaceType();

protected:
    virtual void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Context) = 0;
};

inline void IKLDebugImGuiFeatureInterface_Unique::OnFeatureSelected([[maybe_unused]] const UWorld& _World)
{
}

inline void IKLDebugImGuiFeatureInterface_Unique::OnFeatureUnselected([[maybe_unused]] const UWorld& _World)
{
}

inline EImGuiInterfaceType IKLDebugImGuiFeatureInterface_Unique::GetInterfaceType()
{
    return EImGuiInterfaceType::UNIQUE;
}

inline void IKLDebugImGuiFeatureInterface_Unique::Render([[maybe_unused]] const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const
{
}
