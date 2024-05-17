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
    UE_NODISCARD KLDEBUGUSERFRAMEWORK_API TOptional<uint32> BroadcastGetDebugServerSocketPort();
    UE_NODISCARD KLDEBUGUSERFRAMEWORK_API bool IsGetDebugServerSocketPortBound();

}    // namespace KL::Debug::Client::Delegates::Internal