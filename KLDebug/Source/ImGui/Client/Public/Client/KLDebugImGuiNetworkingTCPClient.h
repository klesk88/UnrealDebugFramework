// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientWorldCacheConnection.h"

// modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingConnectionDelegates.h"
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPBase.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"

class FArchive;
class FKLDebugImGuiClientGameThreadContext;
class UKLDebugNetworkingArbitrerSettings;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiNetworkingTCPClient final : public FKLDebugImGuiNetworkingTCPBase
{
public:
    // FKLDebugImGuiNetworkingTCPBase
    bool Init() final;
    void RunChild() final;
    void CreateSocket() final;
    void Exit() final;
    // FKLDebugImGuiNetworkingTCPBase

    void TickGameThread(FKLDebugImGuiClientGameThreadContext& _Context);

private:
    void CreateArbitrerReplySocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings);

    void RemoveCachedConnection(const int32 _Index);
    void TickReadArbitrerData();
    void TickConnections();

    void OnReadArbitrerData(FArchive& _Reader);

    void GameThread_RemoveInvalidWorlds(const FKLDebugImGuiClientGameThreadContext& _Context);
    void GameThread_NewWorlds(const FKLDebugImGuiClientGameThreadContext& _Context);
    void GameThread_TickImGuiData(FKLDebugImGuiClientGameThreadContext& _Context);

private:
    TArray<FKLDebugImGuiClientWorldCacheConnection> mCachedConnections;
    TSharedPtr<FInternetAddr> mArbitrerAddress;
    TSharedPtr<FInternetAddr> mTempAddress;
    TArray<uint8> mArbitrerTempWriteBuffer;
    TArray<uint8> mArbitrerWriteBuffer;
    TArray<uint8> mArbitrerReplyReadData;
    TArray<uint8> mArbitrerReplyTempData;
    FSocket* mArbitrerReplySocket = nullptr;
    uint32 mArbitrerReplyPort = 0;
};

inline bool FKLDebugImGuiNetworkingTCPClient::Init()
{
    return FKLDebugImGuiNetworkingTCPBase::Init() && mArbitrerReplySocket != nullptr;
}
