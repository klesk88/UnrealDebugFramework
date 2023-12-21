// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"

class ISocketSubsystem;

namespace KL::Debug::Networking::Helpers
{
    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD TSharedRef<FInternetAddr> GetDebugAddress(ISocketSubsystem& _SocketSubsystem);
    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD FIPv4Address GetDebugIPAddress();
}    // namespace KL::Debug::Networking::Helpers