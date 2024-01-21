#pragma once

#include "Internal/Networking/Server/KLDebugNetworkingServerDelegatesClasses.h"
#include "Networking/Server/KLDebugNetworkingServerDelegates.h"

// engine
#include "Delegates/DelegateCombinations.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"

namespace KL::Debug::Server::Delegates::Internal
{
    KLDEBUGUSERFRAMEWORK_API void BindOnGetDebugServerSocketPortRange(const FOnGetDebugServerSocketPortRange& _Delegate);
    KLDEBUGUSERFRAMEWORK_API void UnbindOnGetDebugServerSocketPortRange();
    // without _API on purpose as it should be called just internally this module
    KLDEBUGUSERFRAMEWORK_API UE_NODISCARD TOptional<KL::Debug::Server::Delegates::FServerSocketPortRangeDelegateData> BroadcastOnGetDebugServerSocketPortRange();

}    // namespace KL::Debug::Server::Delegates::Internal