// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Commands/Manager/KLDebugNetworkingCommandConnectionManagerBase.h"

// engine
#include "UObject/ObjectKey.h"

class UWorld;

namespace KL::Debug::Networking::Commands::Internal
{
    class FKLDebugOnNewCommandInput;
}

class KLDEBUGIMGUICLIENT_API FKLDebugNetworkingClientCommandConnectionManager final : public FKLDebugNetworkingCommandConnectionManagerBase
{
public:
    explicit FKLDebugNetworkingClientCommandConnectionManager(const FObjectKey& _WorldKey);
    ~FKLDebugNetworkingClientCommandConnectionManager();

    void GameThread_Tick(UWorld& _CurrentWorld);

private:
    void OnNewCommandRequest(const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& _Input);

private:
    FObjectKey mWorldKey;
};
