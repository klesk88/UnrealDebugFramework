// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

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
    // FKLDebugImGuiNetworkingTCPBase

    void TickGameThread(FKLDebugImGuiTCPServerGameThreadContext& _Context);

private:
    void RemoveCachedConnection(const int32 _Index, FArchive& _ArbitrerWriter);

    void Parallel_TickCachedConnections();
    void Parallel_TickArbitrerConnection();

    void GameThread_InitLocalAddress(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    void GameThread_RemoveOldWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    void GameThread_AddNewWorlds(const FKLDebugImGuiTCPServerGameThreadContext& _Context);
    UE_NODISCARD bool GameThread_UpdateConnections(const FKLDebugImGuiTCPServerGameThreadContext& _Context);

    UE_NODISCARD FSocket* GetNewWorldSocket(const UKLDebugImGuiNetworkingSettings& _Settings, int32& _DebugPort) const;
    UE_NODISCARD const TSharedPtr<FInternetAddr> GetWorldLocalAddress(const UWorld& _World) const;

    void ArbitrerNewWorldConnectionAdded(const int32 _ServerPort, const int32 _DebugPort, FArchive& _ArbitrerWriter);
    void ArbitrerRemovedWorldConnection(const int32 _ServerPort, FArchive& _ArbitrerWriter);

private:
    TArray<FKLDebugImGuiServerWorldCachedConnection> mWorldCachedConnections;
    TSharedPtr<FInternetAddr> mArbitrerAddress;
    TArray<uint8> mArbitrerTempMessageBuffer;
    TArray<uint8> mArbitrerTempBuffer;
    // local addres can be different )for example 127.0.0.1 or 0.0.0.0)
    FIPv4Address mLocalAddress = FIPv4Address{ TNumericLimits<uint32>::Max() };
};

inline bool FKLDebugImGuiNetworkingTCPServer::IsValid() const
{
    return true;
}