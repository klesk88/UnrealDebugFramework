// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Helpers/KLDebugNetworkingHelpers.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"

namespace KL::Debug::Networking::Helpers
{
    //////////////////////////////////////////////
    /// private

    static constexpr uint32 LoopBackAddrMask = 0xff000000;
    static constexpr uint32 LoopBackAddr = 0x7f000000;    // loopback
    static constexpr uint32 LocalAddr = 0x7f000001;       // 127.0.0.1

    //////////////////////////////////////////////
    /// public

    void ChangeAddressToLocalIfLoopback(TSharedRef<FInternetAddr> _Address)
    {
        uint32 RemoteIP = TNumericLimits<uint32>::Max();
        _Address->GetIp(RemoteIP);
        // If this address is on loop back interface, advertise it as 127.0.0.1
        if ((RemoteIP & LoopBackAddrMask) == LoopBackAddr)
        {
            SetAddressToLocal(_Address);
        }
    }

    void SetAddressToLocal(TSharedRef<FInternetAddr> _Address)
    {
        _Address->SetIp(LocalAddr);
    }

}    // namespace KL::Debug::Networking::Helpers