// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

class FKLDebugImGuiFeatureImGuiInput_Unique;
class FKLDebugImGuiFeatureRenderInput_Unique;
class UWorld;

/*
 * This class can be used for data which has a unique representation, meaning there is only one instance of it, during the lifetime of the UWorld.
 * For example this could be Unreal Subsystems, singletons, etc.
 *
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugImGuiFeatureInterface_Unique
 * {
 *   ...
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
    // should return true if the object passed as input is supported by this feature
    UE_NODISCARD virtual bool DoesSupportObject(const UObject& _Object) const = 0;

    virtual void DrawImGui(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input);
    virtual void Render(const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const;

    static UE_NODISCARD EImGuiInterfaceType GetInterfaceType();

protected:
    virtual void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Context) = 0;
};

inline EImGuiInterfaceType IKLDebugImGuiFeatureInterface_Unique::GetInterfaceType()
{
    return EImGuiInterfaceType::UNIQUE;
}

inline void IKLDebugImGuiFeatureInterface_Unique::Render([[maybe_unused]] const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const
{
}
