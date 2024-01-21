#include "Networking/Server/KLDebugNetworkingServerDelegates.h"

#include "Internal/Networking/Server/KLDebugNetworkingServerDelegatesInternal.h"

namespace KL::Debug::Server::Delegates
{
    /////////////////////////////////////////////////////////////////////////
    /// public

    void BindOnGetDebugServerSocketPortRange(const FOnGetDebugServerSocketPortRange& _Delegate)
    {
        KL::Debug::Server::Delegates::Internal::BindOnGetDebugServerSocketPortRange(_Delegate);
    }

    void UnbindOnGetDebugServerSocketPortRange()
    {
        KL::Debug::Server::Delegates::Internal::UnbindOnGetDebugServerSocketPortRange();
    }

}    // namespace KL::Debug::Server::Delegates