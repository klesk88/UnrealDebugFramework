#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterface_Subsystem.h"

class UObject;

enum class KLDEBUGIMGUIUSER_API EObjectSubsytemType : uint8
{
    EDITOR = 0,
    GAME_INSTANCE,
    LOCAL_PLAYER,
    WORLD
};

/*
 * This interface can be extended for subsystems which are based on UObjects (currently everyone except Engine)
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeatureInterface_ObjectSubsystem : public IKLDebugImGuiFeatureInterface_Subsystem
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_ObjectSubsystem, IKLDebugImGuiFeatureInterface_Subsystem)

public:
    ~IKLDebugImGuiFeatureInterface_ObjectSubsystem();

    // should return true if the object passed as input is supported by this feature
    UE_NODISCARD virtual bool DoesSupportObject(const UObject& _Object) const = 0;

    UE_NODISCARD virtual EObjectSubsytemType GetType() const = 0;
};
