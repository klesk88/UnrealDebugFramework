// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui user module
#include "User/Framework/Public/Filter/Interface/KLDebugFilterInterface.h"

class KLDEBUGEXAMPLES_API KLDebugImGuiFilterPlayer final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(Player)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};
