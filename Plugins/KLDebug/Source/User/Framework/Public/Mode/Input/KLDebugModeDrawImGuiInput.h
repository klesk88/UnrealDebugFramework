// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Mode/Input/KLDebugModeInputBase.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugModeDrawImGuiInput final : public FKLDebugModeInputBase
{
public:
    explicit FKLDebugModeDrawImGuiInput(const UWorld& _World, IKLDebugContextInterface* _Context);
};
