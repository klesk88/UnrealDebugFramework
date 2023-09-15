#pragma once

#include "Feature/Interface/Private/Context/KLDebugImGuiFeatureInterfaceRenderContext_Base.h"

class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceRenderContext_Subsystem final : public FKLDebugImGuiFeatureInterfaceRenderContext_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceRenderContext_Subsystem(const UWorld& _World);
};
