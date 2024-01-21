#include "Internal/Networking/Server/KLDebugNetworkingServerDelegatesClasses.h"

namespace KL::Debug::Server::Delegates
{
    /////////////////////////////////////////////////////////////////////////
    /// public

    FServerSocketPortRangeDelegateData::FServerSocketPortRangeDelegateData(const uint32 _StartRange, const uint32 _EndRange)
        : mStartRange(_StartRange)
        , mEndRange(_EndRange)
    {
    }

}    // namespace KL::Debug::Server::Delegates