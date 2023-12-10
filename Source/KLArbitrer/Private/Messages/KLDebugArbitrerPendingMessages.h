// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "Networking/Arbitrer/Public/Messages/Client/KLDebugNetworkingArbitrerMessage_ClientConnected.h"

// engine
#include "HAL/PlatformTime.h"

class FArchive;
class FKLDebugNetworkingArbitrerMessage_ClientConnected;

class FKLDebugArbitrerPendingMessages
{
public:
    explicit FKLDebugArbitrerPendingMessages(FArchive& _Reader);

    UE_NODISCARD const FKLDebugNetworkingArbitrerMessage_ClientConnected& GetMessage() const;
    UE_NODISCARD double GetTimeSinceAdded() const;

private:
    FKLDebugNetworkingArbitrerMessage_ClientConnected mMessage;
    double mCheckTimer;
};

inline const FKLDebugNetworkingArbitrerMessage_ClientConnected& FKLDebugArbitrerPendingMessages::GetMessage() const
{
    return mMessage;
}

inline double FKLDebugArbitrerPendingMessages::GetTimeSinceAdded() const
{
    const double TimeSpan = FPlatformTime::Seconds() - mCheckTimer;
    return TimeSpan;
}
