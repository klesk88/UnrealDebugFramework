// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ServerPortGetter/Arbitrer/KLDebugImGuiClientArbitrerCacheConnection.h"
#include "ServerPortGetter/KLDebugImGuiClientServerPortGetterBase.h"

// engine
#include "Containers/Array.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"
#include "Templates/UnrealTemplate.h"

class FArchive;
class FKLDebugImGuiClientGameThreadContext;
class FKLDebugImGuiClientWorldCacheConnection;
class FKLDebugNetworkingArbitrerMessage_ClientServerData;
class FSocket;
class UKLDebugImGuiNetworkingSettings;
class UKLDebugNetworkingArbitrerSettings;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientServerPortGetterArbitrer final : public IKLDebugImGuiClientServerPortGetterBase
{
private:
    enum class EClientArbitrerState : uint8
    {
        PendingConnection = 0,
        PendingAnswer,
        Connected
    };

public:
    // IKLDebugImGuiClientServerPortGetterBase
    void Init() final;
    void AddNewConnections(const FKLDebugImGuiClientGameThreadContext& _Context) final;
    void ParallelTick(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections) final;
    void Shutdown() final;
    // IKLDebugImGuiClientServerPortGetterBase

private:
    void CreateArbitrerReplySocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings);

    void TickPendingArbitrerConnections();
    void TickReadArbitrerData(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections);

    void OnReadArbitrerData(FArchive& _Reader, TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections);
    void OnArbitrerMessageRecv(const FKLDebugImGuiClientArbitrerCacheConnection& _ArbitrerConnection, const UKLDebugImGuiNetworkingSettings& _Settings, const FKLDebugNetworkingArbitrerMessage_ClientServerData& _Data, TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections);

private:
    TArray<FKLDebugImGuiClientArbitrerCacheConnection> mArbitrerConnections;
    TSharedPtr<FInternetAddr> mArbitrerAddress;
    TSharedPtr<FInternetAddr> mTempAddress;
    TArray<uint8> mArbitrerTempWriteBuffer;
    TArray<uint8> mArbitrerWriteBuffer;
    TArray<uint8> mArbitrerReplyReadData;
    TArray<uint8> mArbitrerReplyTempData;
    FSocket* mArbitrerSocket = nullptr;
    uint32 mArbitrerReplyPort = 0;
    EClientArbitrerState mState = EClientArbitrerState::PendingConnection;
};