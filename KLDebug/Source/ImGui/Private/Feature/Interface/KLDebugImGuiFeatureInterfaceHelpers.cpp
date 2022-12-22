#include "Feature/Interface/KLDebugImGuiFeatureInterfaceHelpers.h"

//engine
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UObject/Object.h"

namespace KL::Debug::ImGui::Feature::Helpers
{
    bool IsPlayer(const UObject& _Object)
    {
        const AController* Controller = GetController(_Object);
        return Cast<const APlayerController>(Controller) != nullptr;
    }

    bool IsAI(const UObject& _Object)
    {
        const AController* Controller = GetController(_Object);
        return Cast<const AAIController>(Controller) != nullptr;
    }

    const AController* GetController(const UObject& _Object)
    {
        const APawn* Pawn = GetPawn(_Object);
        return Pawn ?Pawn->GetController() : nullptr;
    }

    const APawn* GetPawn(const UObject& _Object)
    {
        return Cast<const APawn>(&_Object);
    }
}
