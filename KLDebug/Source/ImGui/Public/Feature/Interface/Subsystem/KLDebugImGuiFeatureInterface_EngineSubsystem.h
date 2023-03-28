#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterface_Subsystem.h"

/*
 * This interface can be extended for subsystems which extend subsystems which are not based on Uobject
 * (Currently only Engine)
 */
class KLDEBUGIMGUI_API IKLDebugImGuiFeatureInterface_EngineSubsystem : public IKLDebugImGuiFeatureInterface_Subsystem
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_EngineSubsystem, IKLDebugImGuiFeatureInterface_Subsystem)
};
