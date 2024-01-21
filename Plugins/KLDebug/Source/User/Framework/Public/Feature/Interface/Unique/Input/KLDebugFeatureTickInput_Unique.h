// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureTickInput_Base.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureTickInput_Unique final : public FKLDebugFeatureTickInput_Base
{
public:
    explicit FKLDebugFeatureTickInput_Unique(const UWorld& _World, IKLDebugContextInterface* _Context);
};
