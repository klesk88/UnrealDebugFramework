// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Delegates/DelegateCombinations.h"

// delegate to determine if the environment we are running is a local host environment (servers running in the same machine)
// or a remote environment (servers running on a remote machine)
DECLARE_DELEGATE_RetVal(bool, FIsServerLocalHost);

namespace KL::Debug::Networking::Delegates
{
    KLDEBUGNETWORKINGRUNTIME_API void BindIsServerLocalHost(const FIsServerLocalHost& _Delegate);
    // NOTE: by default we return true
    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD bool BroadcastIsServerLocalHost();
}    // namespace KL::Debug::Networking::Delegates