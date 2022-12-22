#pragma once

// engine
#include "CoreMinimal.h"

class AController;
class APawn;
class UObject;

namespace KL::Debug::ImGui::Feature::Helpers
{
    KLDEBUGIMGUI_API UE_NODISCARD bool IsPlayer(const UObject& _Object);
    KLDEBUGIMGUI_API UE_NODISCARD bool IsAI(const UObject& _Object);

    KLDEBUGIMGUI_API UE_NODISCARD const AController* GetController(const UObject& _Object);
    KLDEBUGIMGUI_API UE_NODISCARD const APawn* GetPawn(const UObject& _Object);
}