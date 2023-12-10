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

class FArchive;
class FInternetAddr;
class FKLDebugImGuiClientData;
class FKLDebugNetworkingArbitrerMessage_ClientServerData;
class FSocket;
class UKLDebugImGuiNetworkingSettings;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientWorldCacheConnection final : public FNoncopyable
{
private:
    enum class EClientState : uint8
    {
        PendingSendMessageArbitrer = 0,
        HasSentMessageToArbitrer,
        MessageReceveid
    };

public:
    explicit FKLDebugImGuiClientWorldCacheConnection(const UWorld& _World, const FString& _Host, const uint32 _WorldPort, TSharedRef<FInternetAddr> _ArbitrerAddress, FSocket& _ArbitrerSocket);
    ~FKLDebugImGuiClientWorldCacheConnection();

    UE_NODISCARD bool operator==(const FObjectKey& _WorldID) const;
    UE_NODISCARD bool operator==(const uint16& _ID) const;

    void OnArbitrerMessageRecv(const UKLDebugImGuiNetworkingSettings& _Settings, const FKLDebugNetworkingArbitrerMessage_ClientServerData& _Data, FInternetAddr& _TempAddress);

    UE_NODISCARD bool Parallel_Tick(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer);
    UE_NODISCARD bool TickOnGameThread(FKLDebugImGuiClientData& _ClientData);

private:
    void TickArbitrer(const uint32 _ArbitrerReplyPort, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer);

private:
    TUniquePtr<FKLDebugImGuiClientServerCacheConnection> mCacheConnection;
    FObjectKey mWorldKey;
    FString mHost;
    TSharedPtr<FInternetAddr> mArbitrerAddress;
    FSocket* mArbitrerSocket = nullptr;
    double mSentArbitrerMessageTime = 0.0;
    uint32 mWorldPort = 0;
    uint16 mID = 0;
    EClientState mState = EClientState::PendingSendMessageArbitrer;
};

inline bool FKLDebugImGuiClientWorldCacheConnection::operator==(const FObjectKey& _WorldID) const
{
    return mWorldKey == _WorldID;
}

inline bool FKLDebugImGuiClientWorldCacheConnection::operator==(const uint16& _ID) const
{
    return mID == _ID;
}
