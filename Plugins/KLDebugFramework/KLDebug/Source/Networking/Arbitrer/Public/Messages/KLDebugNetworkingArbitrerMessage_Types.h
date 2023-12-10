#pragma once

// engine
#include "GenericPlatform/GenericPlatform.h"

enum class KLDEBUGNETWORKINGARBITRER_API EKLDebugArbitrerMessage : uint8
{
    ClientRegistration = 0,
    Client_GetServerData,
    ServerRegistration,
    ServerDisconnect,
    ServerClientInfo
};
