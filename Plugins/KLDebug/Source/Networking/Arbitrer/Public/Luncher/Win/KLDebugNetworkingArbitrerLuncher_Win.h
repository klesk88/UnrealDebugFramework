// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#if PLATFORM_WINDOWS

// engine
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"

namespace KL::Debug::Networking::Arbitrer
{
    [[nodiscard]] bool LunchArbitrer(const FString& _ExecutablePath, const bool _CreateConsole);
    void CloseArbitrer();
    [[nodiscard]] bool IsArbitrerRunningInternal();
}    // namespace KL::Debug::Networking::Arbitrer

#endif