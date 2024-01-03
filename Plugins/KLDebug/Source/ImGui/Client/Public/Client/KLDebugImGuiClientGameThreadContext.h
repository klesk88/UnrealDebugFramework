// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Subsystem/Engine/KLDebugImGuiNetworkingGameThreadUpdateContextBase.h"

class FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientGameThreadContext final : public FKLDebugImGuiNetworkingGameThreadUpdateContextBase
{
public:
    explicit FKLDebugImGuiClientGameThreadContext();
};
