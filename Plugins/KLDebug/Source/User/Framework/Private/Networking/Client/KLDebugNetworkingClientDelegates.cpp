// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Networking/Client/KLDebugNetworkingClientDelegates.h"

#include "Internal/Networking/Client/KLDebugNetworkingClientDelegatesInternal.h"

namespace KL::Debug::Client::Delegates
{
    //////////////////////////////////////////////////////
    /// public

    void BindGetDebugServerSocketPort(const FGetDebugServerSocketPort& _Delegate)
    {
        KL::Debug::Client::Delegates::Internal::BindGetDebugServerSocketPort(_Delegate);
    }

    void UnbindGetDebugServerSocketPort()
    {
        KL::Debug::Client::Delegates::Internal::UnbindGetDebugServerSocketPort();
    }

}    // namespace KL::Debug::Client::Delegates