// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Server/KLDebugImGuiServerClientCachedConnection.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class FInternetAddr;
class FSocket;
class ISocketSubsystem;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerWorldCachedConnection final : public FNoncopyable
{
private:
    enum class EServerWorldConnectionState : uint8
    {
        MessageToSend = 0,
        MessageSent
    };

public:
    explicit FKLDebugImGuiServerWorldCachedConnection(const UWorld& _World, const int32 _ServerPort, const int32 _DebugPort, FSocket& _WorldDebugSocket);
    ~FKLDebugImGuiServerWorldCachedConnection();
    UE_NODISCARD bool operator==(const FObjectKey& _WorldKey) const;

    void OnRemove(const FInternetAddr& _ArbitrerAddress, FSocket& _ArbitrerSocket, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer);

    void RemoveInvalidClientConnections();

    UE_NODISCARD bool Parallel_Tick(const FInternetAddr& _ArbitrerAddress, FSocket& _ArbitrerSocket, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer);
    UE_NODISCARD bool TickOnGameThread();

private:
    void ShutdownInternal(ISocketSubsystem& _SocketSubsystem);
    void SendArbitrerMessageIfNeeded(const FInternetAddr& _ArbitrerAddress, FSocket& _ArbitrerSocket, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer);

    void TickPendingConnections();
    UE_NODISCARD bool TickClientsConnections();

private:
    TArray<FKLDebugImGuiServerClientCachedConnection> mCachedClientConnections;
    FObjectKey mWorldKey;
    int32 mServerPort = 0;
    int32 mDebugPort = 0;
    EServerWorldConnectionState mState = EServerWorldConnectionState::MessageToSend;
    FSocket* mWorldDebugSocket = nullptr;
};

inline bool FKLDebugImGuiServerWorldCachedConnection::operator==(const FObjectKey& _WorldKey) const
{
    return mWorldKey == _WorldKey;
}
