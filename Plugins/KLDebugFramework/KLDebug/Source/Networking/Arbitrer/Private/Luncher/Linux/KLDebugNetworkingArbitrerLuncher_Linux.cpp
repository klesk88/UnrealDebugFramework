// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Luncher/Linux/KLDebugNetworkingArbitrerLuncher_Linux.h"

#if PLATFORM_UNIX

#include "Definitions/KLDebugNetworkingArbitrerDefinitions.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"

// engine
#include "Containers/UnrealString.h"

namespace KL::Debug::Networking::Arbitrer
{
    /////////////////////////////////////////////////////
    /// private

    /////////////////////////////////////////////////////
    /// public

    void LunchArbitrer(const FString& _ExecutablePath, const bool _CreateConsole)
    {
        ensureMsgf(false, TEXT("not implemented need a linux machine to test"));
    }

    void CloseArbitrer()
    {
        ensureMsgf(false, TEXT("not implemented need a linux machine to test"));
    }

    bool IsArbitrerRunningInternal()
    {
        return false;
    }

}    // namespace KL::Debug::Networking::Arbitrer

#endif