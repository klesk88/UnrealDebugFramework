// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientServerCacheConnection.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class FKLDebugImGuiClientData;
class FSocket;

struct FUniqueNetIdRepl;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientWorldCacheConnection final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientWorldCacheConnection(const FObjectKey& _WorldKey, const FUniqueNetIdRepl& _LocalPlayerNetID, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _WorldServerSocket);
    UE_NODISCARD bool operator==(const FObjectKey& _WorldID) const;

    UE_NODISCARD bool Parallel_Tick();
    UE_NODISCARD bool TickOnGameThread(FKLDebugImGuiClientData& _ClientData);

private:
    FKLDebugImGuiClientServerCacheConnection mCacheConnection;
    FObjectKey mWorldKey;
};

inline bool FKLDebugImGuiClientWorldCacheConnection::operator==(const FObjectKey& _WorldID) const
{
    return mWorldKey == _WorldID;
}
