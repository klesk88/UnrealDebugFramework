#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Interface/Subsystem/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Subsystem.h"
#include "Feature/Interface/Subsystem/Input/KLDebugImGuiFeatureInterfaceRenderInput_Subsystem.h"

class UWorld;

/*
 * This interface can be extended from subsystems
 * NOTE: do not extend directly this class. Please use FKLDebugImGuiFeatureInterface_ObjectSubsystem or FKLDebugImGuiFeatureInterface_EngineSubsystem
 */
class KLDEBUGIMGUI_API IKLDebugImGuiFeatureInterface_Subsystem : public IKLDebugImGuiFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_Subsystem, IKLDebugImGuiFeatureInterfaceBase)

public:
    virtual void DrawImGui(const FKLDebugImGuiFeatureInterfaceImGuiInput_Subsystem& _Context);
    virtual void Render(const FKLDebugImGuiFeatureInterfaceRenderInput_Subsystem& _Context) const;

protected:
    virtual void DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Subsystem& _Context) = 0;
};

inline void IKLDebugImGuiFeatureInterface_Subsystem::Render([[maybe_unused]] const FKLDebugImGuiFeatureInterfaceRenderInput_Subsystem& _Context) const
{
}
