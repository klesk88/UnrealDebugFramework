#include "Delegates/KLDebugImGuiServerDelegates.h"

namespace KL::Debug::Server::Delegates
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

    // without _API on purpose as it should be called just internally this module
    void BroadcastOnGetDebugServerSocketPortRange(uint32& /*_StartRange*/, uint32& /*_EndRange*/);

    TOptional<FServerSocketPortRangeDelegateData> BroadcastOnGetDebugServerSocketPortRange()
    {
        if (OnGetDebugServerSocketPortRangeDelegate.IsBound())
        {
            return OnGetDebugServerSocketPortRangeDelegate.Execute();
        }

        return TOptional<FServerSocketPortRangeDelegateData>();
    }

    FServerSocketPortRangeDelegateData::FServerSocketPortRangeDelegateData(const uint32 _StartRange, const uint32 _EndRange)
        : mStartRange(_StartRange)
        , mEndRange(_EndRange)
    {
    }

}    // namespace KL::Debug::Server::Delegates