// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Delegates/KLDebugNetworkingDelegates.h"

namespace KL::Debug::Networking::Delegates
{
    //////////////////////////////////////////////////////
    /// private

    FIsServerLocalHost IsLocalHostDelegate;

    //////////////////////////////////////////////////////
    /// public

    void BindIsServerLocalHost(const FIsServerLocalHost& _Delegate)
    {
        ensureMsgf(!IsLocalHostDelegate.IsBound(), TEXT("Only one delegate allowed"));

        IsLocalHostDelegate = _Delegate;
    }

    bool BroadcastIsServerLocalHost()
    {
        if (IsLocalHostDelegate.IsBound())
        {
            return IsLocalHostDelegate.Execute();
        }

        return true;
    }

}    // namespace KL::Debug::Networking::Delegates
