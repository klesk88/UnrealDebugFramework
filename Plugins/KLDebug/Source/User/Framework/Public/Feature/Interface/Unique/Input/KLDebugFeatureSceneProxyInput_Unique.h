// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureSceneProxyInput_Base.h"

class UObject;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureSceneProxyInput_Unique final : public FKLDebugFeatureSceneProxyInput_Base
{
public:
    explicit FKLDebugFeatureSceneProxyInput_Unique(const UPrimitiveComponent& _RenderingComponent, IKLDebugContextInterface* _Context);
};
