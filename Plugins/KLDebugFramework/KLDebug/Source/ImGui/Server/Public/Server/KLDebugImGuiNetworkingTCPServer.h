// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Arbitrer/KLDebugImGuiServerArbitrerManager.h"
#include "Server/KLDebugImGuiServerWorldCachedConnection.h"

// modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPBase.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"
#include "Math/NumericLimits.h"
#include "Templates/SharedPointer.h"

class FArchive;
class FKLDebugImGuiTCPServerGameThreadContext;
class FSocket;
class UKLDebugImGuiNetworkingSettings;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiNetworkingTCPServer final : public FKLDebugImGuiNetworkingTCPBase
{
public:
    // FKLDebugImGuiNetworkingTCPBase
    void RunChild() final;
    UE_NODISCARD bool IsValid() const final;
    void Exit() final;
    // FKLDebugImGuiNetworkingTCPBase

    void InitArbitrer();
    void TickGameThread(FKLDebugImGuiTCPServerGameThreadContext& _Context);

private:
    void RemoveCachedConnection(const int32 _Index);

    void Parallel_TickCachedConnections();
    void Parallel_TickArbitrerConnection();

    void GameThread_RemoveOldWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    void GameThread_AddNewWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    UE_NODISCARD bool GameThread_UpdateConnections(const FKLDebugImGuiTCPServerGameThreadContext& _Context);

    UE_NODISCARD FSocket* GetNewWorldSocket(const uint32 _StartPort, const uint32 _EndPort, const UKLDebugImGuiNetworkingSettings& _Settings, int32& _DebugPort) const;
    UE_NODISCARD const TSharedPtr<FInternetAddr> GetWorldLocalAddress(const UWorld& _World) const;

private:
    FKLDebugImGuiServerArbitrerManager mArbitrerManager;
    TArray<FKLDebugImGuiServerWorldCachedConnection> mWorldCachedConnections;
};

inline bool FKLDebugImGuiNetworkingTCPServer::IsValid() const
{
    return true;
}

inline void FKLDebugImGuiNetworkingTCPServer::InitArbitrer()
{
    mArbitrerManager.Init();
}