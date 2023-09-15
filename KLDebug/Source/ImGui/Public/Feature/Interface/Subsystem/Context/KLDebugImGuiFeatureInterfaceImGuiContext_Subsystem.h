#pragma once

#include "Feature/Interface/Private/Context/KLDebugImGuiFeatureInterfaceImGuiContext_Base.h"

class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceImGuiContext_Subsystem final : public FKLDebugImGuiFeatureInterfaceImGuiContext_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiContext_Subsystem(const UWorld& _World, bool& _IsWindowOpen);
};
