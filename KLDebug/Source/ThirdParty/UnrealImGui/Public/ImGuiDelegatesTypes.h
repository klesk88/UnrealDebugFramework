#pragma once

//engine
#include "CoreMinimal.h"
#include <Delegates/Delegate.h>

//@Begin KLMod: Change delegates from FSimpleMulticastDelegate to _World
DECLARE_MULTICAST_DELEGATE_OneParam(FOnImGuiDelegate, const UWorld& /*_World*/);