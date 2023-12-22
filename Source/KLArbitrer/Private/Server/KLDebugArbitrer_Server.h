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

/*
 * This is the arbitrer which will run in a separate cmd line process. This should run on the server machine
 * and it allows the clients to be able to establish a connection with the game servers they are connected to.
 * Because both server and clients will choose a port which is inside a range, this allows to have a service
 * with a static port that both can connect to and use to communicate
 * NOTE: server uses UDP connection as it is mean to live in the same machine of the arbitrer so there is no need
 * of using TCP
 */
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
