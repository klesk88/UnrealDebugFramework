// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ServerPortGetter/User/KLDebugImGuiClientServerPortGetterUser_PendingWorld.h"

// engine
#include "Engine/World.h"

FKLDebugImGuiClientServerPortGetterUser_PendingWorld::FKLDebugImGuiClientServerPortGetterUser_PendingWorld(const UWorld& _World, const uint32 _ServerIP, const FUniqueNetIdRepl& _LocalPlayerNetID)
    : mWorldKey(&_World)
    , mLocalPlayerNetID(_LocalPlayerNetID)
    , mServerIP(_ServerIP)
{
}
