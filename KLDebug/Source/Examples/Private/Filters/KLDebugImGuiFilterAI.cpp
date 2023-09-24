#include "Filters/KLDebugImGuiFilterAI.h"

// engine
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "UObject/Object.h"

KL_DEBUG_CREATE_FILTER(KLDebugImGuiFilterAI)

bool KLDebugImGuiFilterAI::IsObjectValid(const UObject& _Object) const
{
    const APawn* Pawn = Cast<const APawn>(&_Object);
    return Pawn && !Pawn->IsPlayerControlled();
}
