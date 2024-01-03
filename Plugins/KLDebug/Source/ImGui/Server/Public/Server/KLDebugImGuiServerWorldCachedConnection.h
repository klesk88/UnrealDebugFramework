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

namespace KL::Debug::Networking::Commands::Internal
{
    class FKLDebugOnNewCommandInput;
}

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerWorldCachedConnection final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerWorldCachedConnection(const UWorld& _World, const int32 _ServerPort, const int32 _DebugPort, FSocket& _WorldDebugSocket);
    ~FKLDebugImGuiServerWorldCachedConnection();
    UE_NODISCARD bool operator==(const FObjectKey& _WorldKey) const;

    void RemoveInvalidClientConnections();

    UE_NODISCARD int32 GetServerPort() const;

    UE_NODISCARD bool Parallel_Tick();
    UE_NODISCARD bool TickOnGameThread();

private:
    void ShutdownInternal(ISocketSubsystem& _SocketSubsystem);

    void TickPendingConnections();
    UE_NODISCARD bool TickClientsConnections();

    void RemoveInvalidConnections();

    void OnNewCommandRequest(const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& _Input);

private:
    TArray<FKLDebugImGuiServerClientCachedConnection> mCachedClientConnections;
    FObjectKey mWorldKey;
    int32 mServerPort = 0;
    int32 mDebugPort = 0;
    FSocket* mWorldDebugSocket = nullptr;
};

inline bool FKLDebugImGuiServerWorldCachedConnection::operator==(const FObjectKey& _WorldKey) const
{
    return mWorldKey == _WorldKey;
}

inline int32 FKLDebugImGuiServerWorldCachedConnection::GetServerPort() const
{
    return mServerPort;
}