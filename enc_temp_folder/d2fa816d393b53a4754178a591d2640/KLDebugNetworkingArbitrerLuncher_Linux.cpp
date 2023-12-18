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
    }

    void CloseArbitrer()
    {
    }
}    // namespace KL::Debug::Networking::Arbitrer

#endif