#pragma once

//engine
#include "Misc/NetworkGuid.h"

class UObject;
class UWorld;

namespace KL::Debug::ImGuiNetworking::Helpers
{
    UE_NODISCARD FNetworkGUID TryGetNetworkGuid(const UObject& _Object);
    UE_NODISCARD const UObject* TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID);
} // KL::Debug::ImGuiNetworking::Helpers
