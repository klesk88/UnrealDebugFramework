// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Helpers/KLDebugNetworkingHelpers.h"

#include "Delegates/KLDebugNetworkingDelegates.h"

// engine
#include "Containers/UnrealString.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

namespace KL::Debug::Networking::Helpers
{
    TSharedRef<FInternetAddr> GetDebugAddress(ISocketSubsystem& _SocketSubsystem)
    {
        TSharedPtr<FInternetAddr> ArbitrerIP = _SocketSubsystem.CreateInternetAddr();
        const FIPv4Address Address = GetDebugIPAddress();
        const FString AddressString = Address.ToString();
        bool IsValid = false;
        ArbitrerIP->SetIp(*AddressString, IsValid);
        checkf(IsValid, TEXT("address must be valid"));
        return ArbitrerIP.ToSharedRef();
    }

    FIPv4Address GetDebugIPAddress()
    {
        // initialization of IP address based on
        // https://forums.unrealengine.com/t/getting-data-through-udp-connection-no-data-received/286980
        // "Is the sender on the same computer? Your UDP socket is bound to localhost (127.0.0.1) - if you intend to receive packets
        // from other machines you should bind to one of your local IP addresses or to 0.0.0.0 for “any adapter”".

        if (KL::Debug::Networking::Delegates::BroadcastIsServerLocalHost())
        {
            return FIPv4Address::InternalLoopback;
        }
        else
        {
            return FIPv4Address::Any;
        }
    }

}    // namespace KL::Debug::Networking::Helpers