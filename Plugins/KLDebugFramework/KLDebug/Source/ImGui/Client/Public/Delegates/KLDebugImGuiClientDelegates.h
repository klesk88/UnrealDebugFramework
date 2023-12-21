// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Delegates/DelegateCombinations.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"

DECLARE_DELEGATE_RetVal(uint32, FGetDebugServerSocketPort);

namespace KL::Debug::Client::Delegates
{
    KLDEBUGIMGUICLIENT_API void BindGetDebugServerSocketPort(const FGetDebugServerSocketPort& _Delegate);
    KLDEBUGIMGUICLIENT_API void UnbindGetDebugServerSocketPort();
    // without _API on purpose as it should be called just internally this module
    UE_NODISCARD TOptional<uint32> BroadcastGetDebugServerSocketPort();
    UE_NODISCARD bool IsGetDebugServerSocketPortBound();

}    // namespace KL::Debug::Client::Delegates