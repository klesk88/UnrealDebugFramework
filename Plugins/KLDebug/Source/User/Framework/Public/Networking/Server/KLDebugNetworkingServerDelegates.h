#pragma once

#include "Internal/Networking/Server/KLDebugNetworkingServerDelegatesClasses.h"

// engine
#include "Delegates/DelegateCombinations.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"

DECLARE_DELEGATE_RetVal(KL::Debug::Server::Delegates::FServerSocketPortRangeDelegateData, FOnGetDebugServerSocketPortRange);

namespace KL::Debug::Server::Delegates
{
    //////////////////////////////////////////////////////
    /// methods

    KLDEBUGUSERFRAMEWORK_API void BindOnGetDebugServerSocketPortRange(const FOnGetDebugServerSocketPortRange& _Delegate);
    KLDEBUGUSERFRAMEWORK_API void UnbindOnGetDebugServerSocketPortRange();

}    // namespace KL::Debug::Server::Delegates