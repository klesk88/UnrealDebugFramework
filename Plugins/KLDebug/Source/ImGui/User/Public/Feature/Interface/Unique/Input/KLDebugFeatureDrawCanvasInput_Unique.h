// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Input/KLDebugFeatureDrawCanvasInput_Base.h"

class KLDEBUGIMGUIUSER_API FKLDebugFeatureDrawCanvasInput_Unique final : public FKLDebugFeatureDrawCanvasInput_Base
{
public:
    explicit FKLDebugFeatureDrawCanvasInput_Unique(UCanvas& _Canvas, UFont& _Font, FKLDebugImGuiFeatureContext_Base* _Context);
};
