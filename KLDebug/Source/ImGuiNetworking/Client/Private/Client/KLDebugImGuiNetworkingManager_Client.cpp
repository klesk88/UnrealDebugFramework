#include "Client/KLDebugImGuiNetworkingManager_Client.h"

//imgui module
#include "ImGui/Public/Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "ImGui/Public/Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
//networking runtime module
#include "ImGuiNetworking/Runtime/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"
#include "ImGuiNetworking/Runtime/Public/Interface/KLDebugImGuiNetworking_FeatureInterface.h"
#include "ImGuiNetworking/Runtime/Public/Settings/KLDebugImGuiNetworkingSettings.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Common/TcpSocketBuilder.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "Engine/NetDriver.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IPAddress.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/BitReader.h"
#include "Serialization/BitWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Stats/Stats2.h"
#include "Templates/SharedPointer.h"
#include "UObject/CoreNet.h"
#include "UObject/NameTypes.h"

#if !NO_LOGGING
//engine
#include "Containers/UnrealString.h"
#endif

void FKLDebugImGuiNetworkingManager_Client::InitChild(UWorld& _World)
{
    const UNetDriver* ServerNetDriver = _World.GetNetDriver();
    const UNetConnection* NetConnection = ServerNetDriver ? ServerNetDriver->ServerConnection : nullptr;
    if (!NetConnection)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingManager_Client::Init>> Server Connection Is Not Initialized"));
        return;
    }

    const UKLDebugImGuiNetworkingSettings& NetworkingConfig = GetNetworkConfig();
    mReconnectionTime = NetworkingConfig.GetClientReconnectionTime();

    UE_LOG(LogKL_Debug, Display, TEXT("FKLDebugImGuiNetworkingManager_Client::Init>> Trying to connecting to IP: [%s] Port: [%d]"),
        *NetConnection->URL.Host,
        NetworkingConfig.GetPort());

    InitServerSocket(TEXT("ClientSocket"), NetConnection->URL.Host, NetworkingConfig.GetPort(), NetworkingConfig.GetReadBufferSize(), NetworkingConfig.GetWriteBufferSize());

    if (!mServerSocket)
    {
        UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiNetworkingManager_Client::Init>> Connection to IP: [%s] Port: [%d] failed"),
            *NetConnection->URL.Host,
            NetworkingConfig.GetPort());

        return;
    }

    InitTick(_World);

    UE_LOG(LogKL_Debug, Display, TEXT("FKLDebugImGuiNetworkingManager_Client::Init>> Connection to IP: [%s] Port: [%d] succeded"),
        *NetConnection->URL.Host,
        NetworkingConfig.GetPort());


    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = GetWorld().GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    if (!ImGuiWorldSubsystem)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingManager_Client::Init>> UKLDebugImGuiWorldSubsystem not found"));
        return;
    }

    FOnImGuiFeatureStateUpdated::FDelegate FeatureUpdateDelagate = FOnImGuiFeatureStateUpdated::FDelegate::CreateRaw(this, &FKLDebugImGuiNetworkingManager_Client::OnFeatureUpdate);
    mOnFeaturesUpdatedDelegateHandle = ImGuiWorldSubsystem->BindOnImGuiFeatureStateUpdated(FeatureUpdateDelagate);
}

void FKLDebugImGuiNetworkingManager_Client::InitServerSocket(const FString& _SocketName, const FString& _IP, const int32 _Port, const int32 _ReceiveBufferSize, const int32 _SendBufferSize)
{
    //based on FSocketImitationTrainer::FSocketImitationTrainer line 518

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingManager_Client::InitServerSocket>>Could not get socket subsystem"));
        return;
    }

    bool IsValid = false;
    TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();
    Address->SetIp(*_IP, IsValid);
    if (!IsValid)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingManager_Client::InitServerSocket>> Invalid Ip Address [%s]"), *_IP);
        return;
    }

    Address->SetPort(_Port);
    mServerSocket = FTcpSocketBuilder(TEXT("KLDebugImguiClient"))
        .AsNonBlocking()
        .AsReusable()
        .Build();

    if (!mServerSocket)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingManager_Client::InitServerSocket>> Failed to create socket"));
        return;
    }

    int32 ReceiveBufferSize = 0;
    mServerSocket->SetReceiveBufferSize(_ReceiveBufferSize, ReceiveBufferSize);
    mServerSocket->SetSendBufferSize(_SendBufferSize, mSendBufferSize);

    mReceiverDataBuffer.SetNumUninitialized(ReceiveBufferSize);

    mServerAddress = Address;
    mServerSocket->Connect(*Address);
}

void FKLDebugImGuiNetworkingManager_Client::OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Client_OnFeatureUpdate);

    const AActor* ObjectAsActor = Cast<const AActor>(_FeatureUpdateData.TryGetObject());
    if (ObjectAsActor && ObjectAsActor->GetLocalRole() == ROLE_Authority)
    {
        UE_LOG(LogKL_Debug, Display, TEXT("FKLDebugImGuiNetworkingManager_Client::OnFeatureUpdate>> actor [%s] is locally controlled no message sent to server"), 
            *ObjectAsActor->GetName());

        return;
    }

    TArray<TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, FName>> FeaturesIndexes;
    FeaturesIndexes.Reserve(_FeatureUpdateData.GetFeatureIterator().GetFeaturesCount());

    FKLDebugImGuiSubsetFeaturesConstIterator& FeaturesIterator = _FeatureUpdateData.GetFeatureIterator();
    for (; FeaturesIterator; ++FeaturesIterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeaturesIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        const IKLDebugImGuiNetworking_FeatureInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
        if (!NetworkInterface || !NetworkInterface->Client_InformServerWhenActive())
        {
            continue;
        }

        FeaturesIndexes.Emplace(FeaturesIterator.GetFeatureDataIndex(), FeaturesIterator.GetFeatureNameID());
        if (_FeatureUpdateData.IsFullyRemoved())
        {
            break;
        }
    }

    if (FeaturesIndexes.IsEmpty())
    {
        return;
    }

    FNetworkGUID NetworkID;
    if (const FNetworkGUID* NetworkIDMap = mObjectToNetworkID.Find(_FeatureUpdateData.GetObjectKey()))
    {
        NetworkID = *NetworkIDMap;
    }
    else
    {
        if (!_FeatureUpdateData.TryGetObject())
        {
            ensureMsgf(false, TEXT("no valid object passed should not be possible"));
            return;
        }

        NetworkID = KL::Debug::ImGuiNetworking::Helpers::TryGetNetworkGuid(*_FeatureUpdateData.TryGetObject());
        if (!NetworkID.IsValid())
        {
            ensureMsgf(false, TEXT("no valid network ID"));
            return;
        }

        mObjectToNetworkID.Emplace(_FeatureUpdateData.GetObjectKey(), NetworkID);
    }

    //i dont expect mPendingFeaturesStatusUpdates to have elements but just in case 
    FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate* FeatureUpdate = nullptr;
    for (FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& Update : mPendingFeaturesStatusUpdates)
    {
        if (Update.Client_IsEqual(_FeatureUpdateData.GetContainerType(), NetworkID))
        {
            FeatureUpdate = &Update;
            break;
        }
    }
    
    if (!FeatureUpdate)
    {
        FeatureUpdate = &mPendingFeaturesStatusUpdates.Emplace_GetRef(NetworkID);
    }

    if (_FeatureUpdateData.IsFullyRemoved())
    {
        FeatureUpdate->Client_SetFullyRemoved();
    }
    else
    {
        //clear the flag just in case we reenable before send this packet
        FeatureUpdate->Client_ClearFullyRemoved();

        for (const TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, FName>& FeatureIndexPair : FeaturesIndexes)
        {
            FeatureUpdate->Client_AddFeatureUpdate(FeatureIndexPair.Key, FeatureIndexPair.Value, _FeatureUpdateData.IsFeatureAdded());
        }
    }
}

void FKLDebugImGuiNetworkingManager_Client::ClearChild()
{
    if (mServerSocket)
    {
        mServerSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(mServerSocket);
        mServerSocket = nullptr;
    }

    mServerAddress.Reset();

    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = GetWorld().GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    if (ImGuiWorldSubsystem && mOnFeaturesUpdatedDelegateHandle.IsValid())
    {
        ImGuiWorldSubsystem->UnbindOnImGuiFeatureStateUpdated(mOnFeaturesUpdatedDelegateHandle);
    }
}

void FKLDebugImGuiNetworkingManager_Client::Tick(const float _DeltaTime)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Client_Tick);

    if (!mServerSocket)
    {
        return;
    }

    const ESocketConnectionState ConnState = mServerSocket->GetConnectionState();
    switch (ConnState)
    {
    case ESocketConnectionState::SCS_Connected:
        TickReadData();
        TickWriteData();
        break;
    case ESocketConnectionState::SCS_NotConnected:
    case ESocketConnectionState::SCS_ConnectionError:
        TryReconnect();
        break;
    default:
        ensureMsgf(false, TEXT("not implemented"));
    }
}

void FKLDebugImGuiNetworkingManager_Client::TickReadData()
{   
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Client_TickReadData);

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

void FKLDebugImGuiNetworkingManager_Client::TickWriteData()
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Client_TickWriteData);

    //for eplications of actors or actor guid look at UObject* FindReplicatedObjectOnPIEServer
    UPackageMap* PackageMap = GetServerPackageMap();
    if(!PackageMap)
    {
        return;
    }

    FNetBitWriter Writer(PackageMap, mSendBufferSize * 8);
    WritePendingFeaturesStatusUpdate(Writer);

    static_cast<void>(SendData(*mServerSocket, Writer));
}

void FKLDebugImGuiNetworkingManager_Client::TryReconnect()
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Client_TryReconnect);

    if (!mServerAddress.IsValid())
    {
        return;
    }

    const UWorld& World = GetWorld();
    const float TimeSinceLastConnTry = World.TimeSince(mLastTimeTryToConnect);
    if (TimeSinceLastConnTry > mReconnectionTime)
    {
        mLastTimeTryToConnect = World.GetTimeSeconds();
        mServerSocket->Connect(*mServerAddress.Get());
    }
}

void FKLDebugImGuiNetworkingManager_Client::WritePendingFeaturesStatusUpdate(FNetBitWriter& _BitWriter)
{
    if (mPendingFeaturesStatusUpdates.IsEmpty())
    {
        return;
    }

    const UWorld& World = GetWorld();
    for (FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& UpdateStatus : mPendingFeaturesStatusUpdates)
    {
        if (UpdateStatus.CanWrite(World))
        {
            UpdateStatus.Write(World, _BitWriter);
        }
    }

    mPendingFeaturesStatusUpdates.Reset();
}

void FKLDebugImGuiNetworkingManager_Client::ReadData(FBitReader& _Reader)
{
    int32 a = 0;
    _Reader << a;
    a = 40;
}

UPackageMap* FKLDebugImGuiNetworkingManager_Client::GetServerPackageMap() const
{
    UNetDriver* ServerNetDriver = GetWorld().GetNetDriver();
    UNetConnection* NetConnection = ServerNetDriver ? ServerNetDriver->ServerConnection : nullptr;
    if (NetConnection)
    {
        return NetConnection->PackageMap;
    }

    return nullptr;
}
