#include "Server/KLDebugImGuiNetworkingManager_Server.h"

//imgui module
#include "ImGui/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
//networking runtime module
#include "ImGuiNetworking/Runtime/Public/Message/Client/KLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate.h"
#include "ImGuiNetworking/Runtime/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"
#include "ImGuiNetworking/Runtime/Public/Settings/KLDebugImGuiNetworkingSettings.h"
//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Common/TcpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/NetDriver.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/BitReader.h"
#include "Serialization/BitWriter.h"
#include "Stats/Stats2.h"

#if DO_ENSURE
#include "Kismet/KismetSystemLibrary.h"
#endif

void FKLDebugImGuiNetworkingManager_Server::InitChild(UWorld& _World)
{
    ensureMsgf(UKismetSystemLibrary::IsServer(&_World) && !UKismetSystemLibrary::IsStandalone(&_World), TEXT("this should be called only from servers"));

    const UKLDebugImGuiNetworkingSettings& NetworkingConfig =  GetNetworkConfig();
    InitListenerSocket(TEXT("ServerSocket"), NetworkingConfig.GetPort(), NetworkingConfig.GetReadBufferSize());
    mClientWriteBufferSize = NetworkingConfig.GetWriteBufferSize();
    mClientReadBufferSize = NetworkingConfig.GetReadBufferSize();
    if (mListenerSocket)
    {
        InitFeatureContainerMap();
        InitTick(_World);
    }
}

void FKLDebugImGuiNetworkingManager_Server::InitListenerSocket(const FString& _SocketName, const int32 _Port, const int32 _ReceiveBufferSize)
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

void FKLDebugImGuiNetworkingManager_Server::InitFeatureContainerMap()
{
    //client and server can be out of sync in the sense that we can have a deployed server which was built with a
    //different version then the one of the client. The Features indexes in this case could not match anymore.
    //so make a pass here once so we store 

    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Server_InitFeatureContainerMap);

    UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::GetMutable();
    check(ImGuiEngineSubsystem != nullptr);
    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

    for (int32 i = 0; i < static_cast<int32>(EContainerType::COUNT); ++i)
    {

    }
}

void FKLDebugImGuiNetworkingManager_Server::ClearChild()
{
    if (mListenerSocket)
    {
        mListenerSocket->Close();
        mListenerSocket = nullptr;
    }

    mConnectedSockets.Reset();
}

void FKLDebugImGuiNetworkingManager_Server::Tick(const float _DeltaTime)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Server_Tick);

    if (!mListenerSocket)
    {
        return;
    }

    TickListenerSocket();
    TickConnections();
}

void FKLDebugImGuiNetworkingManager_Server::TickListenerSocket()
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Server_TickListenerSocket);

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
            mConnectedSockets.Emplace(TRefCountPtr<FKLDebugImGuiNetworkingCacheConnection>(new FKLDebugImGuiNetworkingCacheConnection(*ConnectionSocket)));
        }
        else
        {
            UE_LOG(LogKL_Debug, Warning,
                TEXT("Listen socket received a pending connection event but ListenSocket->Accept ")
                TEXT("failed to create a ClientSocket."));
        }
    }
}

UPackageMap* FKLDebugImGuiNetworkingManager_Server::GetClientPackageMap(const UWorld& _World, const FSocket& _ClientSocket) const
{
    const UNetDriver* ServerNetDriver = _World.GetNetDriver();
    if (ServerNetDriver && ServerNetDriver->ClientConnections.Num() > 0)
    {
        return ServerNetDriver->ClientConnections[0]->PackageMap;
    }

    return nullptr;
}

void FKLDebugImGuiNetworkingManager_Server::TickConnections()
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Server_TickConnections);

    for (int32 i = mConnectedSockets.Num() - 1; i >= 0; --i)
    {
        TRefCountPtr<FKLDebugImGuiNetworkingCacheConnection>& CacheConnection = mConnectedSockets[i];
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

        ReceiveConnectionData(*CacheConnection, ClientSocket);
        SendConnectionData(ClientSocket);
    }
}

void FKLDebugImGuiNetworkingManager_Server::ReceiveConnectionData(FKLDebugImGuiNetworkingCacheConnection& _Connection, FSocket& _ClientSocket)
{
    uint32 Size = 0;
    int32 BytesRead = 0;

    const UWorld& World = GetWorld();
    while (_ClientSocket.HasPendingData(Size) && Size > 0)
    {
        const uint32 MaxReadSize = FMath::Min(Size, static_cast<uint32>(mReceiverDataBuffer.Num()));
        _ClientSocket.Recv(mReceiverDataBuffer.GetData(), MaxReadSize, BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKL_Debug, Log, TEXT("Socket had pending data but didnt read any bytes"));
            continue;
        }
     
        UPackageMap* PackageMap = GetClientPackageMap(World, _ClientSocket);
        FNetBitReader Reader(PackageMap, mReceiverDataBuffer.GetData(), BytesRead * 8);
        ReadData(_Connection, Reader);
    }
}

void FKLDebugImGuiNetworkingManager_Server::ReadData(FKLDebugImGuiNetworkingCacheConnection& _Connection, FBitReader& _Reader)
{
    const UWorld& World = GetWorld();

    UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::GetMutable();
    check(ImGuiEngineSubsystem != nullptr);
    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

    while (!_Reader.AtEnd())
    {
        EKLDebugNetworkMessageTypes MessageType = EKLDebugNetworkMessageTypes::Count;
        _Reader << MessageType;
    
        switch (MessageType)
        {
        case EKLDebugNetworkMessageTypes::Client_FeatureStatusUpdate:
            HandleClientFeatureStatusUpdate(FeatureContainerManager, World, _Connection, _Reader);
            break;
        case EKLDebugNetworkMessageTypes::Count:
            ensureMsgf(false, TEXT("not handled"));
            break;
        }
    }
}

void FKLDebugImGuiNetworkingManager_Server::HandleClientFeatureStatusUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FKLDebugImGuiNetworkingCacheConnection& _Connection, FBitReader& _Reader)
{
    FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate FeatureStatusUpdate;
    FeatureStatusUpdate.Read(_World, _Reader);

    if (FeatureStatusUpdate.Server_IsFullyRemoved())
    {
        _Connection.RemoveObjectFeatures(FeatureStatusUpdate.Server_GetNetworkID());
    }
    else
    {
        FKLDebugImGuiNetworking_ServerObjectFeatures& ServerObjectFeatureData = _Connection.GetOrAddFeaturesPerObject(_World, FeatureStatusUpdate.Server_GetNetworkID());
        FKLDebugImGuiNetworking_ServerObjectContainerFeatures& FeatureContainer = ServerObjectFeatureData.GetContainerMutable(FeatureStatusUpdate.Server_GetContainerType());
        FeatureContainer.InitIfNeeded();

        const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainerManager.GetContainer(FeatureStatusUpdate.Server_GetContainerType());

        for (const FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::FeatureData& FeatureDataPair : FeatureStatusUpdate.Server_GetFeaturesData())
        {
            ensureMsgf(Container.IsValidFeatureIndex(FeatureDataPair.Key), TEXT("Feature is not valid for this container"));

            if (FeatureDataPair.Value)
            {
                FeatureContainer.AddFeature(FeatureDataPair.Key);
            }
            else
            {
                FeatureContainer.RemoveFeature(FeatureDataPair.Key);
            }
        }
    }
}

void FKLDebugImGuiNetworkingManager_Server::SendConnectionData(FSocket& _ClientSocket) const
{
    //FNetBitWriter Writer(mClientWriteBufferSize * 8);
    //static int32 a = 25;
    //Writer << a;
    //a++;

    //static_cast<void>(SendData(_ClientSocket, Writer));
}
