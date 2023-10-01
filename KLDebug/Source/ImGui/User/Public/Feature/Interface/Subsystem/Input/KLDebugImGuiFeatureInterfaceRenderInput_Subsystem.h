#pragma once

#include "Feature/Interface/Input/KLDebugImGuiFeatureInterfaceRenderInput_Base.h"

class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureInterfaceRenderInput_Subsystem final : public FKLDebugImGuiFeatureInterfaceRenderInput_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceRenderInput_Subsystem(const UWorld& _World);
};
