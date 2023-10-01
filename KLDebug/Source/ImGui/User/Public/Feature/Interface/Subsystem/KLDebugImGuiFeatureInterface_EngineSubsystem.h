#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterface_Subsystem.h"

/*
 * This interface can be extended for subsystems which extend subsystems which are not based on Uobject
 * (Currently only Engine)
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeatureInterface_EngineSubsystem : public IKLDebugImGuiFeatureInterface_Subsystem
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_EngineSubsystem, IKLDebugImGuiFeatureInterface_Subsystem)

public:
    ~IKLDebugImGuiFeatureInterface_EngineSubsystem();
};
