// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Input/KLDebugFeatureSceneProxyInput_Base.h"

class FKLDebugImGuiFeatureContext_Base;
class UObject;

class KLDEBUGIMGUIUSER_API FKLDebugFeatureSceneProxyInput_Unique final : public FKLDebugFeatureSceneProxyInput_Base
{
public:
    explicit FKLDebugFeatureSceneProxyInput_Unique(const UPrimitiveComponent& _RenderingComponent, FKLDebugImGuiFeatureContext_Base* _Context);
};
