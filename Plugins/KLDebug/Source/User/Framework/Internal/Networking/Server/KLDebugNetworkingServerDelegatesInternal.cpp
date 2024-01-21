#include "Internal/Networking/Server/KLDebugNetworkingServerDelegatesInternal.h"

#include "Networking/Server/KLDebugNetworkingServerDelegates.h"

namespace KL::Debug::Server::Delegates::Internal
{
    /////////////////////////////////////////////////////////////////////////
    /// private

    FOnGetDebugServerSocketPortRange OnGetDebugServerSocketPortRangeDelegate;

    /////////////////////////////////////////////////////////////////////////
    /// public

    void BindOnGetDebugServerSocketPortRange(const FOnGetDebugServerSocketPortRange& _Delegate)
    {
        ensureMsgf(!OnGetDebugServerSocketPortRangeDelegate.IsBound(), TEXT("we can bound only one delegate"));

        OnGetDebugServerSocketPortRangeDelegate = _Delegate;
    }

    void UnbindOnGetDebugServerSocketPortRange()
    {
        OnGetDebugServerSocketPortRangeDelegate.Unbind();
    }

    TOptional<FServerSocketPortRangeDelegateData> BroadcastOnGetDebugServerSocketPortRange()
    {
        if (OnGetDebugServerSocketPortRangeDelegate.IsBound())
        {
            return OnGetDebugServerSocketPortRangeDelegate.Execute();
        }

        return TOptional<FServerSocketPortRangeDelegateData>();
    }

}    // namespace KL::Debug::Server::Delegates::Internal