// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiClientGameThreadContext;
class FKLDebugImGuiClientWorldCacheConnection;

class KLDEBUGIMGUICLIENT_API IKLDebugImGuiClientServerPortGetterBase : public FNoncopyable
{
public:
    virtual ~IKLDebugImGuiClientServerPortGetterBase() = default;

    virtual void Init() = 0;
    virtual void AddNewConnections(const FKLDebugImGuiClientGameThreadContext& _Context) = 0;
    virtual void ParallelTick(TArray<FKLDebugImGuiClientWorldCacheConnection>& _CachedConnections) = 0;
    virtual void Shutdown() = 0;
};