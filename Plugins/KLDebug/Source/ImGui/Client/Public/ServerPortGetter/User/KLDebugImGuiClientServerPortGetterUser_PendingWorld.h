// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "GameFramework/OnlineReplStructs.h"
#include "Math/NumericLimits.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientServerPortGetterUser_PendingWorld final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientServerPortGetterUser_PendingWorld(const UWorld& _World, const uint32 _ServerIP, const FUniqueNetIdRepl& _LocalPlayerNetID);
    [[nodiscard]] bool operator==(const FObjectKey& _WorldKey) const;

    [[nodiscard]] const FObjectKey& GetWorldKey() const;
    [[nodiscard]] uint32 GetServerIP() const;
    [[nodiscard]] const FUniqueNetIdRepl& GetNetLocalPlayerID() const;

private:
    FObjectKey mWorldKey;
    FUniqueNetIdRepl mLocalPlayerNetID;
    uint32 mServerIP = TNumericLimits<uint32>::Max();
};

inline bool FKLDebugImGuiClientServerPortGetterUser_PendingWorld::operator==(const FObjectKey& _WorldKey) const
{
    return mWorldKey == _WorldKey;
}

inline const FObjectKey& FKLDebugImGuiClientServerPortGetterUser_PendingWorld::GetWorldKey() const
{
    return mWorldKey;
}

inline uint32 FKLDebugImGuiClientServerPortGetterUser_PendingWorld::GetServerIP() const
{
    return mServerIP;
}

inline const FUniqueNetIdRepl& FKLDebugImGuiClientServerPortGetterUser_PendingWorld::GetNetLocalPlayerID() const
{
    return mLocalPlayerNetID;
}