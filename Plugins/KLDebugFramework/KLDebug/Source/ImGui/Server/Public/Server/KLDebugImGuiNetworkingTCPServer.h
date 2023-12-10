// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Server/KLDebugImGuiServerWorldCachedConnection.h"

// modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPBase.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"

class FKLDebugImGuiTCPServerGameThreadContext;
class FSocket;
class UKLDebugImGuiNetworkingSettings;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiNetworkingTCPServer final : public FKLDebugImGuiNetworkingTCPBase
{
public:
    // FKLDebugImGuiNetworkingTCPBase
    void RunChild() final;
    void CreateSocket() final;
    // FKLDebugImGuiNetworkingTCPBase

    void TickGameThread(FKLDebugImGuiTCPServerGameThreadContext& _Context);

private:
    void RemoveCachedConnection(const int32 _Index);

    void Parallel_TickCachedConnections();

    void GameThread_RemoveOldWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    void GameThread_AddNewWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    UE_NODISCARD bool GameThread_UpdateConnections(const FKLDebugImGuiTCPServerGameThreadContext& _Context);

    UE_NODISCARD FSocket* GetNewWorldSocket(const UKLDebugImGuiNetworkingSettings& _Settings, int32& _DebugPort) const;

private:
    TArray<FKLDebugImGuiServerWorldCachedConnection> mWorldCachedConnections;
    TSharedPtr<FInternetAddr> mArbitrerAddress;
    TArray<uint8> mArbitrerTempMessageBuffer;
    TArray<uint8> mArbitrerTempBuffer;
};
