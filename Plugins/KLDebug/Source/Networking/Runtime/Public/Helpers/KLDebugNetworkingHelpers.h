// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "IPAddress.h"
#include "Templates/SharedPointer.h"

class ISocketSubsystem;

namespace KL::Debug::Networking::Helpers
{
    KLDEBUGNETWORKINGRUNTIME_API void ChangeAddressToLocalIfLoopback(TSharedRef<FInternetAddr> _Address);
    KLDEBUGNETWORKINGRUNTIME_API void SetAddressToLocal(TSharedRef<FInternetAddr> _Address);
}    // namespace KL::Debug::Networking::Helpers