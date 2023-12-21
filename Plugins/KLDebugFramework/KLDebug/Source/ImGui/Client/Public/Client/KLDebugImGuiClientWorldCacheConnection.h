// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientServerCacheConnection.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "GenericPlatform/GenericPlatform.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class FKLDebugImGuiClientData;
class FSocket;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientWorldCacheConnection final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientWorldCacheConnection(const FObjectKey& _WorldKey, const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _WorldServerSocket);
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
