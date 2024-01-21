// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureRenderInput_Base.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureRenderInput_Unique final : public FKLDebugFeatureRenderInput_Base
{
public:
    explicit FKLDebugFeatureRenderInput_Unique(const UWorld& _World, IKLDebugContextInterface* _Context);
};
