#include "Internal/Networking/Client/KLDebugNetworkingClientDelegatesInternal.h"

namespace KL::Debug::Client::Delegates::Internal
{
    //////////////////////////////////////////////////////
    /// private

    FGetDebugServerSocketPort GetServerSocketPortDelegate;

    //////////////////////////////////////////////////////
    /// public

    void BindGetDebugServerSocketPort(const FGetDebugServerSocketPort& _Delegate)
    {
        ensureMsgf(!GetServerSocketPortDelegate.IsBound(), TEXT("Only one delegate allowed"));

        GetServerSocketPortDelegate = _Delegate;
    }

    void UnbindGetDebugServerSocketPort()
    {
        GetServerSocketPortDelegate.Unbind();
    }

    TOptional<uint32> BroadcastGetDebugServerSocketPort()
    {
        if (GetServerSocketPortDelegate.IsBound())
        {
            return GetServerSocketPortDelegate.Execute();
        }

        return TOptional<uint32>();
    }

    bool IsGetDebugServerSocketPortBound()
    {
        return GetServerSocketPortDelegate.IsBound();
    }

}    // namespace KL::Debug::Client::Delegates::Internal