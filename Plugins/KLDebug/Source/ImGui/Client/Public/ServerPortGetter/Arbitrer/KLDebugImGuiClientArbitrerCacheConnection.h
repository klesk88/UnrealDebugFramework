// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientServerCacheConnection.h"

// engine
#include "Containers/Array.h"
#include "GameFramework/OnlineReplStructs.h"
#include "GenericPlatform/GenericPlatform.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class FArchive;
class FInternetAddr;
class FKLDebugNetworkingArbitrerMessage_ClientServerData;
class FSocket;
class UKLDebugImGuiNetworkingSettings;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientArbitrerCacheConnection final : public FNoncopyable
{
private:
    enum class EClientState : uint8
    {
        PendingSendMessageArbitrer = 0,
        HasSentMessageToArbitrer,
        MessageReceveid
    };

public:
    explicit FKLDebugImGuiClientArbitrerCacheConnection(const UWorld& _World, const FUniqueNetIdRepl& _LocalPlayerNetID, const uint32 _WorldPort, TSharedRef<FInternetAddr> _ArbitrerAddress, FSocket& _ArbitrerSocket);
    ~FKLDebugImGuiClientArbitrerCacheConnection();
    [[nodiscard]] bool operator==(const uint16& _ID) const;

    void Parallel_Tick(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer);

    [[nodiscard]] uint32 GetHost() const;
    [[nodiscard]] const FObjectKey& GetWorldObjKey() const;
    [[nodiscard]] const FUniqueNetIdRepl& GetNetLocalPlayerID() const;

private:
    void TickArbitrer(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer);

private:
    FObjectKey mWorldKey;
    FUniqueNetIdRepl mLocalPlayerNetID;
    uint32 mHost = 0;
    TSharedPtr<FInternetAddr> mArbitrerAddress;
    FSocket* mArbitrerSocket = nullptr;
    double mSentArbitrerMessageTime = 0.0;
    uint32 mWorldPort = 0;
    uint16 mID = 0;
    EClientState mState = EClientState::PendingSendMessageArbitrer;
};

inline bool FKLDebugImGuiClientArbitrerCacheConnection::operator==(const uint16& _ID) const
{
    return mID == _ID;
}

inline uint32 FKLDebugImGuiClientArbitrerCacheConnection::GetHost() const
{
    return mHost;
}

inline const FObjectKey& FKLDebugImGuiClientArbitrerCacheConnection::GetWorldObjKey() const
{
    return mWorldKey;
}

inline const FUniqueNetIdRepl& FKLDebugImGuiClientArbitrerCacheConnection::GetNetLocalPlayerID() const
{
    return mLocalPlayerNetID;
}