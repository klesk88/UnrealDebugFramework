#include "Networking/Client/KLDebugImGuiNetworkManager_Client.h"

#include "Config/KLDebugImGuiConfig_Networking.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Common/TcpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/BitReader.h"
#include "Serialization/BitWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Templates/SharedPointer.h"
#include "UObject/CoreNet.h"

void FKLDebugImGuiNetworkManager_Client::Init(UWorld& _World)
{
    const UNetDriver* ServerNetDriver = _World.GetNetDriver();
    const UNetConnection* NetConnection = ServerNetDriver ? ServerNetDriver->ServerConnection : nullptr;
    if (!NetConnection)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("KLDebugImGuiNetworkManager_Client::Init>> Server Connection Is Not Initialized"));
        return;
    }

    mWorld = &_World;
    const FKLDebugImGuiConfig_Networking& NetworkingConfig = GetNetworkConfig();

    UE_LOG(LogKL_Debug, Display, TEXT("KLDebugImGuiNetworkManager_Client::Init>> Trying to connecting to IP: [%s] Port: [%d]"),
        *NetConnection->URL.Host,
        NetworkingConfig.GetPort());

    InitServerSocket(TEXT("ClientSocket"), NetConnection->URL.Host, NetworkingConfig.GetPort(), NetworkingConfig.GetReadBufferSize(), NetworkingConfig.GetWriteBufferSize());

    if (mServerSocket)
    {
        InitTick(_World);

        UE_LOG(LogKL_Debug, Display, TEXT("KLDebugImGuiNetworkManager_Client::Init>> Connection to IP: [%s] Port: [%d] succeded"),
            *NetConnection->URL.Host,
            NetworkingConfig.GetPort());
    }
    else
    {
        UE_LOG(LogKL_Debug, Warning, TEXT("KLDebugImGuiNetworkManager_Client::Init>> Connection to IP: [%s] Port: [%d] failed"),
            *NetConnection->URL.Host,
            NetworkingConfig.GetPort());
    }
}

void FKLDebugImGuiNetworkManager_Client::InitServerSocket(const FString& _SocketName, const FString& _IP, const int32 _Port, const int32 _ReceiveBufferSize, const int32 _SendBufferSize)
{
    //based on FSocketImitationTrainer::FSocketImitationTrainer line 518

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("KLDebugImGuiNetworkManager_Client::InitServerSocket>>Could not get socket subsystem"));
        return;
    }

    bool IsValid = false;
    TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();
    Address->SetIp(*_IP, IsValid);
    if (!IsValid)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("KLDebugImGuiNetworkManager_Client::InitServerSocket>> Invalid Ip Address [%s]"), *_IP);
        return;
    }

    Address->SetPort(_Port);
    mServerSocket = FTcpSocketBuilder(TEXT("KLDebugImguiClient"))
        .AsNonBlocking()
        .AsReusable()
        .Build();

    if (!mServerSocket)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("KLDebugImGuiNetworkManager_Client::InitServerSocket>> Failed to create socket"));
        return;
    }

    int32 ReceiveBufferSize = 0;
    mServerSocket->SetReceiveBufferSize(_ReceiveBufferSize, ReceiveBufferSize);
    mServerSocket->SetSendBufferSize(_SendBufferSize, mSendBufferSize);

    mReceiverDataBuffer.SetNumUninitialized(ReceiveBufferSize);

    mServerSocket->Connect(*Address);
}

void FKLDebugImGuiNetworkManager_Client::ClearChild()
{
    if (mServerSocket)
    {
        mServerSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(mServerSocket);
        mServerSocket = nullptr;
    }
}

void FKLDebugImGuiNetworkManager_Client::Tick(const float _DeltaTime)
{
    if (!mServerSocket)
    {
        return;
    }

    const ESocketConnectionState ConnState = mServerSocket->GetConnectionState();
    if (ConnState != SCS_Connected)
    {
        return;
    }

    TickReadData();
    TickWriteData();
}

void FKLDebugImGuiNetworkManager_Client::TickReadData()
{   
    uint32 Size = 0;
    int32 BytesRead = 0;
    while (mServerSocket->HasPendingData(Size) && Size > 0)
    {
        const uint32 MaxReadSize = FMath::Min(Size, static_cast<uint32>(mReceiverDataBuffer.Num()));
        mServerSocket->Recv(mReceiverDataBuffer.GetData(), MaxReadSize, BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKL_Debug, Log, TEXT("Socket had pending data but didnt read any bytes"));
            continue;
        }

        FNetBitReader Reader(nullptr, mReceiverDataBuffer.GetData(), BytesRead * 8);
        ReadData(Reader);
    }
}


#include "Engine/Engine.h"
#include "Engine/NetDriver.h"
#include "GameFramework/PlayerController.h"

void FKLDebugImGuiNetworkManager_Client::TickWriteData()
{
    //for eplications of actors or actor guid look at UObject* FindReplicatedObjectOnPIEServer

    UNetDriver* ServerNetDriver = mWorld->GetNetDriver();
    UPackageMap* PackageMap = nullptr;
    if (UNetConnection* NetConnection = ServerNetDriver->ServerConnection)
    {
        PackageMap = NetConnection->PackageMap;
    }
    else if (ServerNetDriver->ClientConnections.Num() > 0)
    {
        PackageMap = ServerNetDriver->ClientConnections[0]->PackageMap;
    }

    if(!PackageMap)
    {
        return;
    }

    APlayerController* Controller = GEngine->GetFirstLocalPlayerController(mWorld.Get());
    if (!Controller)
    {
        return;
    }

    FNetBitWriter Writer(PackageMap, mSendBufferSize * 8);
    if (hasWritten)
    {
        return;
    }


    Writer << Controller;

    hasWritten = SendData(*mServerSocket, Writer);
}

void FKLDebugImGuiNetworkManager_Client::ReadData(FBitReader& _Reader)
{
    int32 a = 0;
    _Reader << a;
    a = 40;
}