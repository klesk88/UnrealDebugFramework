// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Filter/Interface/KLDebugFilterInterface.h"

class UObject;

class KLDEBUGEXAMPLES_API KLDebugImGuiFilterAI final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(AI)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};
