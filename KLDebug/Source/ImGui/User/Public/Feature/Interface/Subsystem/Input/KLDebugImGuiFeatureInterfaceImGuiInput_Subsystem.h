#pragma once

#include "Feature/Interface/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Base.h"

class FKLDebugImGuiFeatureContext_Base;
class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureInterfaceImGuiInput_Subsystem final : public FKLDebugImGuiFeatureInterfaceImGuiInput_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiInput_Subsystem(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext);
};
