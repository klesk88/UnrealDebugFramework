#include "Server/KLDebugImGuiServerManager.h"

//modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Networking/Public/Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "ImGui/Networking/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "Networking/Runtime/Public/Settings/KLDebugNetworkingSettings.h"
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

#if !WITH_EDITOR
#include "Subsystem/KLDebugImGuiServerSubsystem_Engine.h"
#endif

void FKLDebugImGuiServerManager::InitFromEngine()
{
    const UKLDebugNetworkingSettings& NetworkingConfig = GetNetworkConfig();
    InitListenerSocket(TEXT("ServerSocket"), NetworkingConfig.GetPort(), NetworkingConfig.GetReadBufferSize());
    mClientWriteBufferSize = NetworkingConfig.GetWriteBufferSize();
    mClientReadBufferSize = NetworkingConfig.GetReadBufferSize();
}

void FKLDebugImGuiServerManager::ClearFromEngine()
{
    if (mListenerSocket)
    {
        mListenerSocket->Close();
        mListenerSocket = nullptr;
    }

    mConnectedSockets.Reset();
}

void FKLDebugImGuiServerManager::InitListenerSocket(const FString& _SocketName, const int32 _Port, const int32 _ReceiveBufferSize)
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
    const FIPv4Address IPAddr(127, 0, 0, 1);
    const FIPv4Endpoint Endpoint(IPAddr, _Port);
    mListenerSocket = FTcpSocketBuilder(*_SocketName)
        .AsReusable()
        .AsNonBlocking()
        .BoundToEndpoint(Endpoint)
        .Listening(8)
        .Build();

    if (!mListenerSocket)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("FKLDebugImGuiServerManager::InitListenerSocket>> Failed in creating listener socket"));
        return;
    }

    int32 NewSize = 0;
    mListenerSocket->SetReceiveBufferSize(_ReceiveBufferSize, NewSize);

    mReceiverDataBuffer.SetNumUninitialized(NewSize);
}

void FKLDebugImGuiServerManager::InitFromWorldChild(UWorld& _World)
{
    ensureMsgf(UKismetSystemLibrary::IsServer(&_World) && !UKismetSystemLibrary::IsStandalone(&_World), TEXT("this should be called only from servers"));

    if (mListenerSocket)
    {
        InitTick(_World);
    }
}

void FKLDebugImGuiServerManager::ClearFromWorldChild(const UWorld& _World)
{
}

void FKLDebugImGuiServerManager::Tick(const float _DeltaTime)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Server_Tick);

    if (!mListenerSocket)
    {
        return;
    }

    TickListenerSocket();
    TickConnections();
}

void FKLDebugImGuiServerManager::TickListenerSocket()
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
            mConnectedSockets.Emplace(TRefCountPtr<FKLDebugImGuiServerCacheConnection>(new FKLDebugImGuiServerCacheConnection(*ConnectionSocket)));
        }
        else
        {
            UE_LOG(LogKL_Debug, Warning,
                TEXT("Listen socket received a pending connection event but ListenSocket->Accept ")
                TEXT("failed to create a ClientSocket."));
        }
    }
}

UPackageMap* FKLDebugImGuiServerManager::GetClientPackageMap(const UWorld& _World, const FSocket& _ClientSocket) const
{
    const UNetDriver* ServerNetDriver = _World.GetNetDriver();
    if (ServerNetDriver && ServerNetDriver->ClientConnections.Num() > 0)
    {
        return ServerNetDriver->ClientConnections[0]->PackageMap;
    }

    return nullptr;
}

void FKLDebugImGuiServerManager::TickConnections()
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Server_TickConnections);

    const UWorld& World = GetWorld();

    for (int32 i = mConnectedSockets.Num() - 1; i >= 0; --i)
    {
        TRefCountPtr<FKLDebugImGuiServerCacheConnection>& CacheConnection = mConnectedSockets[i];
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

        const EReadWriteDataResult ReadState = ReceiveConnectionData(*CacheConnection, ClientSocket);
        switch (ReadState)
        {
        case EReadWriteDataResult::Fail:
            //something went wrong drop connection
            mConnectedSockets.RemoveAtSwap(i, 1, false);
            continue;
        case EReadWriteDataResult::Succeeded:
            break;
        }

        SendConnectionData(World, *CacheConnection, ClientSocket);
    }
}

FKLDebugImGuiServerManager::EReadWriteDataResult FKLDebugImGuiServerManager::ReceiveConnectionData(FKLDebugImGuiServerCacheConnection& _Connection, FSocket& _ClientSocket)
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
        const EReadWriteDataResult ReadResult = ReadData(_Connection, Reader);
        if (ReadResult != EReadWriteDataResult::Succeeded)
        {
            return ReadResult;
        }
    }

    return EReadWriteDataResult::Succeeded;
}

FKLDebugImGuiServerManager::EReadWriteDataResult FKLDebugImGuiServerManager::ReadData(FKLDebugImGuiServerCacheConnection& _Connection, FBitReader& _Reader)
{
    const UWorld& World = GetWorld();
    
    UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::GetMutable();
    check(ImGuiEngineSubsystem != nullptr);
    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

    while (!_Reader.AtEnd() && _Reader.GetBytesLeft() > 1)
    {
        EKLDebugNetworkMessageTypes MessageType = EKLDebugNetworkMessageTypes::Count;
        _Reader << MessageType;
    
        EReadWriteDataResult ReadResult = EReadWriteDataResult::Succeeded;
        switch (MessageType)
        {
        case EKLDebugNetworkMessageTypes::Client_FeatureStatusUpdate:
            if (_Connection.Rcv_HandleClientFeatureStatusUpdate(FeatureContainerManager, World, _Reader))
            {
                ReadResult = EReadWriteDataResult::Succeeded;
            }
            else
            {
                ReadResult = EReadWriteDataResult::Fail;
            }
            break;
        case EKLDebugNetworkMessageTypes::Count:
            ensureMsgf(false, TEXT("not handled"));
            return EReadWriteDataResult::Fail;
            break;
        }

        if (ReadResult != EReadWriteDataResult::Succeeded)
        {
            return ReadResult;
        }
    }

    _Reader.SetAtEnd();
    return EReadWriteDataResult::Succeeded;
}

void FKLDebugImGuiServerManager::SendConnectionData(const UWorld& _World, const FKLDebugImGuiServerCacheConnection& _Connection, FSocket& _ClientSocket) const
{
    const UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    if (!ImGuiEngineSubsystem)
    {
        ensureMsgf(false, TEXT("ImGuiEngineSubsystem not present"));
        return;
    }

    const FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainer = ImGuiEngineSubsystem->GetFeatureContainerManager();

    FNetBitWriter Writer(mClientWriteBufferSize * 8);

    _Connection.Write_ConnectionFeatures(_World, FeaturesContainer, Writer);

    if (Writer.GetNumBits() != 0)
    {
        static_cast<void>(SendData(_ClientSocket, Writer));
    }
}
