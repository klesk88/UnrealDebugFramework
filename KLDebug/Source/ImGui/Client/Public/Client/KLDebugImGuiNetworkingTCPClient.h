// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiTCPClientCachedConnection.h"

// modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingConnectionDelegates.h"
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPBase.h"

// engine
#include "Containers/Array.h"
#include "Delegates/IDelegateInstance.h"
#include "HAL/Platform.h"

class FKLDebugImGuiClientGameThreadContext;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiNetworkingTCPClient final : public FKLDebugImGuiNetworkingTCPBase
{
public:
    // FKLDebugImGuiNetworkingTCPBase
    void CreateSocket() final;
    void RunChild() final;
    // FKLDebugImGuiNetworkingTCPBase

    void TickGameThread(FKLDebugImGuiClientGameThreadContext& _Context);

private:
    void InitSocket();

    void RemoveCachedConnection(const int32 _Index);
    void TickPendingConnections();
    void TickConnections();

    void GameThread_RemoveInvalidWorlds(const FKLDebugImGuiClientGameThreadContext& _Context);
    void GameThread_TickImGuiData(FKLDebugImGuiClientGameThreadContext& _Context);

private:
    TArray<FKLDebugImGuiTCPClientCachedConnection> mCachedConnections;
    bool mHasPendingWorlds = false;
};