// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"

DECLARE_DELEGATE_RetVal(bool, FOnArbitrerShouldLunch);

namespace KL::Debug::Networking::Arbitrer
{
    KLDEBUGNETWORKINGARBITRER_API void BindOnArbitrerShouldLunch(const FOnArbitrerShouldLunch& _Delegate);
    KLDEBUGNETWORKINGARBITRER_API void UnbindOnArbitrerShouldLunch();

    [[nodiscard]] KLDEBUGNETWORKINGARBITRER_API bool IsArbitrerSupported();
    [[nodiscard]] KLDEBUGNETWORKINGARBITRER_API bool TryLunchArbitrer();
    KLDEBUGNETWORKINGARBITRER_API void TryCloseArbitrer();
    [[nodiscard]] KLDEBUGNETWORKINGARBITRER_API bool IsArbitrerRunning();
}    // namespace KL::Debug::Networking::Arbitrer
