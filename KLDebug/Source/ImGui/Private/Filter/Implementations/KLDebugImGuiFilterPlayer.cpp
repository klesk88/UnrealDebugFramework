#include "Filter/Implementations/KLDebugImGuiFilterPlayer.h"

// engine
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UObject/Object.h"

KL_DEBUG_CREATE_FILTER(KLDebugImGuiFilterPlayer)

bool KLDebugImGuiFilterPlayer::IsObjectValid(const UObject& _Object) const
{
    const APawn*       Pawn       = Cast<const APawn>(&_Object);
    const AController* Controller = Pawn ? Pawn->GetController() : nullptr;
    return Cast<const APlayerController>(Controller) != nullptr;
}
