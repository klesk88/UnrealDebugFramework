// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Messages/KLDebugArbitrerPendingMessages.h"

// modules
#include "Networking/Arbitrer/Public/Messages/Client/KLDebugNetworkingArbitrerMessage_ClientConnected.h"

FKLDebugArbitrerPendingMessages::FKLDebugArbitrerPendingMessages(FArchive& _Reader)
    : mMessage(_Reader)
{
    mCheckTimer = FPlatformTime::Seconds();
}
