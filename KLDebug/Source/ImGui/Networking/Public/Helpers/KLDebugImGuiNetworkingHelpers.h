// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Misc/NetworkGuid.h"

class UObject;
class UWorld;

namespace KL::Debug::ImGuiNetworking::Helpers
{
    KLDEBUGIMGUINETWORKING_API UE_NODISCARD FNetworkGUID TryGetNetworkGuid(const UObject& _Object);

    KLDEBUGIMGUINETWORKING_API UE_NODISCARD const UObject* TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID);
    KLDEBUGIMGUINETWORKING_API UE_NODISCARD UObject* TryGetObjectFromNetworkGUIDMutable(const UWorld& _World, const FNetworkGUID& _NetworkGUID);
}    // namespace KL::Debug::ImGuiNetworking::Helpers
