// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureSceneProxyInput_Base.h"

class UObject;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureSceneProxyInput_Selectable final : public FKLDebugFeatureSceneProxyInput_Base
{
public:
    explicit FKLDebugFeatureSceneProxyInput_Selectable(const UPrimitiveComponent& _RenderingComponent, IKLDebugContextInterface* _Context);
};
