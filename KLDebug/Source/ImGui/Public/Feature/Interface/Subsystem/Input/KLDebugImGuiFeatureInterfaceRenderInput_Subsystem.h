#pragma once

#include "Feature/Interface/Private/Input/KLDebugImGuiFeatureInterfaceRenderInput_Base.h"

class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceRenderInput_Subsystem final : public FKLDebugImGuiFeatureInterfaceRenderInput_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceRenderInput_Subsystem(const UWorld& _World);
};
