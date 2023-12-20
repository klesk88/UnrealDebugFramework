// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Delegates/DelegateCombinations.h"

DECLARE_DELEGATE_RetVal(bool, FOnArbitrerShouldLunch);

namespace KL::Debug::Networking::Arbitrer
{
    KLDEBUGNETWORKINGARBITRER_API void BindOnArbitrerShouldLunch(const FOnArbitrerShouldLunch& _Delegate);
    KLDEBUGNETWORKINGARBITRER_API void UnbindOnArbitrerShouldLunch();

    KLDEBUGNETWORKINGARBITRER_API void TryLunchArbitrer();
    KLDEBUGNETWORKINGARBITRER_API void TryCloseArbitrer();
    KLDEBUGNETWORKINGARBITRER_API UE_NODISCARD bool IsArbitrerRunning();
}    // namespace KL::Debug::Networking::Arbitrer
