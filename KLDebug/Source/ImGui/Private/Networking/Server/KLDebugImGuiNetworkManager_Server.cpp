#include "Networking/Server/KLDebugImGuiNetworkManager_Server.h"

#include "Config/KLDebugImGuiConfig_Networking.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Common/TcpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/BitReader.h"
#include "Serialization/BitWriter.h"

#if DO_ENSURE
#include "Kismet/KismetSystemLibrary.h"
#endif

void FKLDebugImGuiNetworkManager_Server::Init(UWorld& _World)
{
    ensureMsgf(UKismetSystemLibrary::IsServer(&_World) && !UKismetSystemLibrary::IsStandalone(&_World), TEXT("this should be called only from servers"));

    mWorld = &_World;
    const FKLDebugImGuiConfig_Networking& NetworkingConfig =  GetNetworkConfig();
    InitListenerSocket(TEXT("ServerSocket"), NetworkingConfig.GetPort(), NetworkingConfig.GetReadBufferSize());
    mClientWriteBufferSize = NetworkingConfig.GetWriteBufferSize();
    mClientReadBufferSize = NetworkingConfig.GetReadBufferSize();
    if (mListenerSocket)
    {
        InitTick(_World);
    }
}

void FKLDebugImGuiNetworkManager_Server::InitListenerSocket(const FString& _SocketName, const int32 _Port, const int32 _ReceiveBufferSize)
{
    if (mListenerSocket)
    {
        ensureMsgf(false, TEXT("already initialized"));
        return;
    }

    //if (!FIPv4Address::Parse(TEXT("127.0.0.1"), IPAddr))
    //{
    //    UE_LOG(LogKL_Debug, Log, TEXT("KLDebugImGuiNetworkManager_Base::InitListenerSocket>> IP address [%s] was not valid!"));
    //    return;
    //}

    //Create Socket
    //we are the server so we connect to ourselfs
    FIPv4Address IPAddr(127, 0, 0, 1);
    const FIPv4Endpoint Endpoint(IPAddr, _Port);
    mListenerSocket = FTcpSocketBuilder(*_SocketName)
        .AsReusable()
        .AsNonBlocking()
        .BoundToEndpoint(Endpoint)
        .Listening(8)
        .Build();

    if (!mListenerSocket)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("KLDebugImGuiNetworkManager_Base::InitListenerSocket>> Failed in creating listener socket"));
        return;
    }

    int32 NewSize = 0;
    mListenerSocket->SetReceiveBufferSize(_ReceiveBufferSize, NewSize);

    mReceiverDataBuffer.SetNumUninitialized(NewSize);
}

void FKLDebugImGuiNetworkManager_Server::Tick(const float _DeltaTime)
{
    if (!mListenerSocket)
    {
        return;
    }

    TickListenerSocket();
    TickConnections();
}

void FKLDebugImGuiNetworkManager_Server::ClearChild()
{
    if (mListenerSocket)
    {
        mListenerSocket->Close();
        mListenerSocket = nullptr;
    }
}

void FKLDebugImGuiNetworkManager_Server::TickListenerSocket()
{
    //gather new connections if any
    //based on FEditorDomainSaveServer::PollIncomingConnections

    bool Pending = false;
    // handle incoming connections
    while (mListenerSocket->HasPendingConnection(Pending) && Pending)
    {
        //New Connection receive!
        FSocket* ConnectionSocket = mListenerSocket->Accept(TEXT("Server Socket"));

        if (ConnectionSocket)
        {
            int32 NewSize = 0;
            ConnectionSocket->SetNonBlocking(true);
            ConnectionSocket->SetReceiveBufferSize(mClientReadBufferSize, NewSize);
            ConnectionSocket->SetSendBufferSize(mClientWriteBufferSize, NewSize);
            mConnectedSockets.Emplace(TRefCountPtr<FKLDebugImGuiNetworkCacheConnection>(new FKLDebugImGuiNetworkCacheConnection(*ConnectionSocket)));
        }
        else
        {
            UE_LOG(LogKL_Debug, Warning,
                TEXT("Listen socket received a pending connection event but ListenSocket->Accept ")
                TEXT("failed to create a ClientSocket."));
        }
    }
}

void FKLDebugImGuiNetworkManager_Server::TickConnections()
{
    static bool send = false;
    for (int32 i = mConnectedSockets.Num() - 1; i >= 0; --i)
    {
        TRefCountPtr<FKLDebugImGuiNetworkCacheConnection>& CacheConnection = mConnectedSockets[i];
        if (!CacheConnection->IsValid())
        {
            mConnectedSockets.RemoveAtSwap(i, 1, false);
            continue;
        }

        FSocket& ClientSocket = CacheConnection->GetSocket();
        if (ClientSocket.GetConnectionState() != SCS_Connected)
        {
            mConnectedSockets.RemoveAtSwap(i, 1, false);
            continue;
        }

        ReceiveConnectionData(ClientSocket);
        if (!send)
        {
            SendConnectionData(ClientSocket);
        }
    }

    send = true;
}

#include "Engine/Engine.h"
#include "Engine/NetDriver.h"
#include "GameFramework/PlayerController.h"

void FKLDebugImGuiNetworkManager_Server::ReceiveConnectionData(FSocket& _ClientSocket)
{
    uint32 Size = 0;
    int32 BytesRead = 0;

    while (_ClientSocket.HasPendingData(Size) && Size > 0)
    {
        const uint32 MaxReadSize = FMath::Min(Size, static_cast<uint32>(mReceiverDataBuffer.Num()));
        _ClientSocket.Recv(mReceiverDataBuffer.GetData(), MaxReadSize, BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKL_Debug, Log, TEXT("Socket had pending data but didnt read any bytes"));
            continue;
        }

        UNetDriver* ServerNetDriver = mWorld->GetNetDriver();
        UPackageMap* PackageMap = nullptr;
        if (ServerNetDriver->ClientConnections.Num() > 0)
        {
            PackageMap = ServerNetDriver->ClientConnections[0]->PackageMap;
        }

        FNetBitReader Reader(PackageMap, mReceiverDataBuffer.GetData(), BytesRead * 8);
        ReadData(Reader);
    }
}

void FKLDebugImGuiNetworkManager_Server::ReadData(FBitReader& _Reader)
{
    APlayerController* Controller = nullptr;
    _Reader << Controller;
    Controller = nullptr;
}

void FKLDebugImGuiNetworkManager_Server::SendConnectionData(FSocket& _ClientSocket) const
{
    FNetBitWriter Writer(mClientWriteBufferSize * 8);
    static int32 a = 25;
    Writer << a;
    a++;

    static_cast<void>(SendData(_ClientSocket, Writer));
}
