// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureImGuiInput_Base.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureImGuiInput_Unique final : public FKLDebugFeatureImGuiInput_Base
{
public:
    explicit FKLDebugFeatureImGuiInput_Unique(const UWorld& _World, bool& _IsWindowOpen, IKLDebugContextInterface* _FeatureContext);
};
