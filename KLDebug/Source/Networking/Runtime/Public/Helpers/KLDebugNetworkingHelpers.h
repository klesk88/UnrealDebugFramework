#pragma once

//engine
#include "Misc/NetworkGuid.h"

class UObject;
class UWorld;

namespace KL::Debug::Networking::Helpers
{
    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD FNetworkGUID TryGetNetworkGuid(const UObject& _Object);

    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD const UObject* TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID);
    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD UObject* TryGetObjectFromNetworkGUIDMutable(const UWorld& _World, const FNetworkGUID& _NetworkGUID);
} // KL::Debug::Networking::Helpers
