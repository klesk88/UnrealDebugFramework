// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Input/KLDebugFeatureTickInput_Base.h"

class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugFeatureTickInput_Unique final : public FKLDebugFeatureTickInput_Base
{
public:
    explicit FKLDebugFeatureTickInput_Unique(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _Context);
};
