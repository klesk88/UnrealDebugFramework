// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Misc/NetworkGuid.h"

class UObject;
class UWorld;

namespace KL::Debug::ImGuiNetworking::Helpers
{
    // we cant have const as we access a map inside which as key accept WeakPtr of non const objects
    UE_NODISCARD KLDEBUGIMGUINETWORKING_API FNetworkGUID TryGetNetworkGuid(UObject& _Object);

    UE_NODISCARD KLDEBUGIMGUINETWORKING_API const UObject* TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID);
    UE_NODISCARD KLDEBUGIMGUINETWORKING_API UObject* TryGetObjectFromNetworkGUIDMutable(const UWorld& _World, const FNetworkGUID& _NetworkGUID);
}    // namespace KL::Debug::ImGuiNetworking::Helpers
