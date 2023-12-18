// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

DECLARE_DELEGATE_RetVal(bool, FOnArbitrerShouldLunch);

namespace KL::Debug::Networking::Arbitrer
{
    KLDEBUGNETWORKINGARBITRER_API void BindOnArbitrerShouldLunch(const FOnArbitrerShouldLunch& _Delegate);
    KLDEBUGNETWORKINGARBITRER_API void UnbindOnArbitrerShouldLunch();

    KLDEBUGNETWORKINGARBITRER_API void TryLunchArbitrer();
    KLDEBUGNETWORKINGARBITRER_API void TryCloseArbitrer();
}    // namespace KL::Debug::Networking::Arbitrer
