// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Messages/KLDebugArbitrerPendingMessages.h"
#include "Server/KLDebugArbitrerDebugServerInfo.h"

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "GenericPlatform/GenericPlatform.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"
#include "Templates/UnrealTemplate.h"

class FArchive;
class FKLDebugNetworkingArbitrerMessage_ClientConnected;
class FKLDebugNetworkingMessage_Header;
class FSocket;
class ISocketSubsystem;

class FKLDebugNetworkArbitrer_Server final : public FNoncopyable
{
public:
    UE_NODISCARD bool Init();
    void Run();
    void Exit();

private:
    void CreateSocket();
    void CreateClientSocket();
    void InitListenerSocket();

    void ClearListenerSocket(ISocketSubsystem& _SocketSubsystem);
    void ClearClientSocket(ISocketSubsystem& _SocketSubsystem);

    void TickReadData();
    void TickReadData(FArchive& _Reader);
    void ReadData(const FKLDebugNetworkingMessage_Header& _Header, FArchive& _Reader);

    void TickPendingClientMessages();
    UE_NODISCARD bool TrySendPendingClientMessage(const FKLDebugNetworkingArbitrerMessage_ClientConnected& _Message);

private:
    TArray<FKLDebugArbitrerPendingMessages> mPendingMessages;
    TArray<uint8> mReceiveBuffer;
    TArray<uint8> mTempMessageBuffer;
    TArray<uint8> mTempSendMessageBuffer;
    TArray<uint8> mSendMessageBuffer;
    TSharedPtr<FInternetAddr> mTempClientAddress;
    TMap<uint32, FKLDebugArbitrerDebugServerInfo> mServerPortToDebugPort;
    FSocket* mListenerSocket = nullptr;
    FSocket* mClientSocket = nullptr;
};
