// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Networking/Client/KLDebugNetworkingClientDelegates.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"

namespace KL::Debug::Client::Delegates::Internal
{
    KLDEBUGUSERFRAMEWORK_API void BindGetDebugServerSocketPort(const FGetDebugServerSocketPort& _Delegate);
    KLDEBUGUSERFRAMEWORK_API void UnbindGetDebugServerSocketPort();
    KLDEBUGUSERFRAMEWORK_API UE_NODISCARD TOptional<uint32> BroadcastGetDebugServerSocketPort();
    KLDEBUGUSERFRAMEWORK_API UE_NODISCARD bool IsGetDebugServerSocketPortBound();

}    // namespace KL::Debug::Client::Delegates::Internal