#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"

class UWorld;

/*
 * This interface can be extended from subsystems
 * NOTE: do not extend directly this class. Please use FKLDebugImGuiFeatureInterface_ObjectSubsystem or FKLDebugImGuiFeatureInterface_EngineSubsystem
 */
class KLDEBUGIMGUI_API IKLDebugImGuiFeatureInterface_Subsystem : public IKLDebugImGuiFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_Subsystem, IKLDebugImGuiFeatureInterfaceBase)

public:
    virtual void DrawImGui(const UWorld& _World, bool& _IsWindowOpen);
    virtual void Render(const UWorld& _World) const;

protected:
    virtual void DrawImGuiChild(const UWorld& _World) = 0;
};

inline void IKLDebugImGuiFeatureInterface_Subsystem::Render(const UWorld& _World) const
{
}
