// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Input/KLDebugImGuiFeatureImGuiInput_Base.h"

class FKLDebugImGuiFeatureContext_Base;
class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureImGuiInput_Unique final : public FKLDebugImGuiFeatureImGuiInput_Base
{
public:
    explicit FKLDebugImGuiFeatureImGuiInput_Unique(const UWorld& _World, bool& _IsWindowOpen);
};
