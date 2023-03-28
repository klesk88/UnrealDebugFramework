#pragma once

// imgui module
#include "ImGui/Public/Filter/Interface/KLDebugImGuiFilterInterface.h"

class KLDEBUGEXAMPLES_API KLDebugImGuiFilterPlayer final : public IKLDebugImGuiFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(Player)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};
