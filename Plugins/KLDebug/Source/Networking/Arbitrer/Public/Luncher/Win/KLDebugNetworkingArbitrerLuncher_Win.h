// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#if PLATFORM_WINDOWS

// engine
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"

namespace KL::Debug::Networking::Arbitrer
{
    UE_NODISCARD bool LunchArbitrer(const FString& _ExecutablePath, const bool _CreateConsole);
    void CloseArbitrer();
    UE_NODISCARD bool IsArbitrerRunningInternal();
}    // namespace KL::Debug::Networking::Arbitrer

#endif