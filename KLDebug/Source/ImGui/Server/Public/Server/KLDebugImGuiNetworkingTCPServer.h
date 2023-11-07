#pragma once

#include "Server/KLDebugImGuiTCPServerCachedConnection.h"

//modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPBase.h"

//engine
#include "Containers/Array.h"
#include "Delegates/IDelegateInstance.h"
#include "Engine/World.h"
#include "Misc/NetworkGuid.h"
#include "Math/NumericLimits.h"
#include "Misc/ScopeLock.h"
#include "UObject/ObjectKey.h"

class AController;
class AGameModeBase;
class FKLDebugImGuiTCPServerGameThreadContext;
class FKLDebugImGuiServerGetConnectionsBuffersData;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiNetworkingTCPServer final : public FKLDebugImGuiNetworkingTCPBase
{
public:
    //FKLDebugImGuiNetworkingTCPBase
    void CreateSocket() final;
    void RunChild() final;
    //FKLDebugImGuiNetworkingTCPBase

    void TickGameThread(FKLDebugImGuiTCPServerGameThreadContext& _Context);

private:
    void InitSocket();
    
    void RemoveCachedConnection(const int32 _Index);

    void TickRemovePlayerConnections();
    void TickCachedConnections();

    void GameThread_RemoveOldWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    void GameThread_UpdateNewPlayersConnections(FKLDebugImGuiTCPServerGameThreadContext& _Context);
    void GameThread_RemovePlayersConnection(FKLDebugImGuiTCPServerGameThreadContext& _Context);
    UE_NODISCARD bool GameThread_UpdateConnections(const FKLDebugImGuiTCPServerGameThreadContext& _Context);

private:
    TArray<FKLDebugImGuiTCPServerCachedConnection> mClientsCachedConnection;
    KL::Debug::ImGui::Networking::Server::CacheConnectionID mCacheConnectionsID = 0;
};
