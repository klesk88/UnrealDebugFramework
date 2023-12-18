// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui user module
#include "User/Public/Filter/Interface/KLDebugImGuiFilterInterface.h"

class KLDEBUGEXAMPLES_API KLDebugImGuiFilterPlayer final : public IKLDebugImGuiFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(Player)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};
