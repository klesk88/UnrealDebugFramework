// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "GameFramework/OnlineReplStructs.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class FKLDebugImGuiClientGameThreadContext;
class FKLDebugImGuiClientWorldCacheConnection;
class UWorld;

class KLDEBUGIMGUICLIENT_API IKLDebugImGuiClientServerPortGetterBase : public FNoncopyable
{
public:
    virtual ~IKLDebugImGuiClientServerPortGetterBase() = default;

    virtual void Init() = 0;
    virtual void AddNewConnections(const FKLDebugImGuiClientGameThreadContext& _Context) = 0;
    virtual void ParallelTick(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections) = 0;
    virtual void Shutdown() = 0;
    virtual void RemoveWorlds(const TArray<FObjectKey>& _RemovedWorlds);

    UE_NODISCARD bool GatherWorldData(const UWorld& _World, uint32& _ServerIP, int32& _ServerPort, FUniqueNetIdRepl& _LocalPlayerNetID) const;
    UE_NODISCARD bool IsWorldDataReady(const UWorld& _World) const;
};

inline void IKLDebugImGuiClientServerPortGetterBase::RemoveWorlds(const TArray<FObjectKey>& _RemovedWorlds)
{
}