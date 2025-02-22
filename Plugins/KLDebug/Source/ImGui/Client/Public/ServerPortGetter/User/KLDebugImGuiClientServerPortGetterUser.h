// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ServerPortGetter/KLDebugImGuiClientServerPortGetterBase.h"
#include "ServerPortGetter/User/KLDebugImGuiClientServerPortGetterUser_PendingWorld.h"

// engine
#include "Containers/Array.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiClientGameThreadContext;
class FKLDebugImGuiClientWorldCacheConnection;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientServerPortGetterUser final : public IKLDebugImGuiClientServerPortGetterBase
{
public:
    // IKLDebugImGuiClientServerPortGetterBase
    void Init() final;
    void AddNewConnections(const FKLDebugImGuiClientGameThreadContext& _Context) final;
    void ParallelTick(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections) final;
    void Shutdown() final;
    void RemoveWorlds(const TArray<FObjectKey>& _RemovedWorlds) final;
    // IKLDebugImGuiClientServerPortGetterBase

private:
    void TickPendingWorlds(const uint32 _ServerPort, TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections);

private:
    TArray<FKLDebugImGuiClientServerPortGetterUser_PendingWorld> mPendingWorlds;
    TSharedPtr<FInternetAddr> mTempAddress;
    uint32 mServerPort = 0;
};

inline void FKLDebugImGuiClientServerPortGetterUser::Shutdown()
{
}
