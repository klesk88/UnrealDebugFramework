// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Subsystem/Engine/KLDebugImGuiNetworkingGameThreadUpdateContextBase.h"

class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiTCPServerGameThreadContext final : public FKLDebugImGuiNetworkingGameThreadUpdateContextBase
{
public:
    explicit FKLDebugImGuiTCPServerGameThreadContext(const bool _IsArbitrerRunning);

    UE_NODISCARD bool GetIsArbitrerRunning() const;

private:
    bool IsArbitrerRunning = false;
};

inline bool FKLDebugImGuiTCPServerGameThreadContext::GetIsArbitrerRunning() const
{
    return IsArbitrerRunning;
}