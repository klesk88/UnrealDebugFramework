// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Input/KLDebugImGuiFeatureRenderInput_Base.h"

class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureRenderInput_Unique final : public FKLDebugImGuiFeatureRenderInput_Base
{
public:
    explicit FKLDebugImGuiFeatureRenderInput_Unique(const UWorld& _World);
};
