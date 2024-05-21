// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientServerCacheConnection.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class FSocket;

struct FUniqueNetIdRepl;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientWorldCacheConnection final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientWorldCacheConnection(const FObjectKey& _WorldKey, const FUniqueNetIdRepl& _LocalPlayerNetID, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _WorldServerSocket);
    [[nodiscard]] bool operator==(const FObjectKey& _WorldID) const;

    [[nodiscard]] bool Parallel_Tick();
    [[nodiscard]] bool TickOnGameThread();

private:
    FKLDebugImGuiClientServerCacheConnection mCacheConnection;
    FObjectKey mWorldKey;
};

inline bool FKLDebugImGuiClientWorldCacheConnection::operator==(const FObjectKey& _WorldID) const
{
    return mWorldKey == _WorldID;
}
