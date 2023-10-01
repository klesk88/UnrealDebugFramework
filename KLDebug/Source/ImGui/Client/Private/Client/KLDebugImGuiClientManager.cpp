#include "Client/KLDebugImGuiClientManager.h"

//modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "ImGui/Framework/Public/Feature/Input/KLDebugImGuiGatherFeatureInput.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "ImGui/Networking/Public/Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingReceiveDataInput.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "Networking/Runtime/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"
#include "Networking/Runtime/Public/Settings/KLDebugImGuiNetworkingSettings.h"
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
#include "Serialization/MemoryReader.h"
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

void FKLDebugImGuiClientManager::InitFromWorldChild(UWorld& _World)
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
}

void FKLDebugImGuiClientManager::InitServerSocket(const FString& _SocketName, const FString& _IP, const int32 _Port, const int32 _ReceiveBufferSize, const int32 _SendBufferSize)
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

void FKLDebugImGuiClientManager::InitWorldDelegates()
{
    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = GetWorld().GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    if (!ImGuiWorldSubsystem)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiNetworkingManager_Client::Init>> UKLDebugImGuiWorldSubsystem not found"));
        return;
    }

    FOnImGuiFeatureStateUpdated::FDelegate FeatureUpdateDelagate = FOnImGuiFeatureStateUpdated::FDelegate::CreateRaw(this, &FKLDebugImGuiClientManager::OnFeatureUpdate);
    mOnFeaturesUpdatedDelegateHandle = ImGuiWorldSubsystem->BindOnImGuiFeatureStateUpdated(FeatureUpdateDelagate);
}

void FKLDebugImGuiClientManager::ClearWorldDelegates()
{
    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = GetWorld().GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    if (ImGuiWorldSubsystem && mOnFeaturesUpdatedDelegateHandle.IsValid())
    {
        ImGuiWorldSubsystem->UnbindOnImGuiFeatureStateUpdated(mOnFeaturesUpdatedDelegateHandle);
    }
}

void FKLDebugImGuiClientManager::OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData)
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
        const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
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

        NetworkID = KL::Debug::Networking::Helpers::TryGetNetworkGuid(*_FeatureUpdateData.TryGetObject());
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
        FeatureUpdate = &mPendingFeaturesStatusUpdates.Emplace_GetRef(NetworkID, _FeatureUpdateData.GetContainerType());
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

void FKLDebugImGuiClientManager::ClearFromWorldChild(const UWorld& _World)
{
    if (mServerSocket)
    {
        mServerSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(mServerSocket);
        mServerSocket = nullptr;
    }

    mServerAddress.Reset();

    ClearWorldDelegates();
}

void FKLDebugImGuiClientManager::Tick(const float _DeltaTime)
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
        if (!mHasInitializedAfterConnection)
        {
            InitWorldDelegates();
            mHasInitializedAfterConnection = true;
        }

        TickReadData();
        TickWriteData();
        break;
    case ESocketConnectionState::SCS_NotConnected:
    case ESocketConnectionState::SCS_ConnectionError:
        if (mHasInitializedAfterConnection)
        {
            ClearWorldDelegates();
            mHasInitializedAfterConnection = false;
        }

        TryReconnect();
        break;
    default:
        ensureMsgf(false, TEXT("not implemented"));
    }
}

void FKLDebugImGuiClientManager::TickReadData()
{   
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingManager_Client_TickReadData);

    const UWorld& World = GetWorld();
    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = World.GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    const UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    if (!ImGuiWorldSubsystem || !ImGuiEngineSubsystem)
    {
        ensureMsgf(false, TEXT("not expected"));
        return;
    }

    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

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

        UPackageMap* PackageMap = GetServerPackageMap();
        FNetBitReader Reader(PackageMap, mReceiverDataBuffer.GetData(), BytesRead * 8);
        ReadData(World, FeatureContainerManager, *ImGuiWorldSubsystem, Reader);
    }
}

void FKLDebugImGuiClientManager::TickWriteData()
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

    if (Writer.GetNumBits() != 0)
    {
        static_cast<void>(SendData(*mServerSocket, Writer));
    }
}

void FKLDebugImGuiClientManager::TryReconnect()
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

void FKLDebugImGuiClientManager::WritePendingFeaturesStatusUpdate(FNetBitWriter& _BitWriter)
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

void FKLDebugImGuiClientManager::ReadData(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UKLDebugImGuiWorldSubsystem& _ImGuiWorldSubsystem, FBitReader& _Reader)
{
    while (!_Reader.AtEnd() && _Reader.GetBytesLeft() > 1)
    {
        EKLDebugNetworkMessageTypes MessageType = EKLDebugNetworkMessageTypes::Count;
        _Reader << MessageType;
        switch (MessageType)
        {
        case EKLDebugNetworkMessageTypes::Server_FeatureDataUpdate:
        {
            FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate RcvData;
            RcvData.Read(_World, _Reader);
            FKLDebugImGuiGatherFeatureInput Input{ RcvData.GetFeatureIndex(), EContainerType::SELECTABLE_OBJECTS, RcvData.GetObject(_World), _FeatureContainerManager };
            _ImGuiWorldSubsystem.TryGatherFeatureAndContext(Input);
            IKLDebugImGuiFeatureInterfaceBase* FeatureInterface = Input.TryGetFeatureInterface();
            IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface ? FeatureInterface->TryGetNetworkInterfaceMutable() : nullptr;
            if (NetworkInterface)
            {
                FMemoryReader MemoryReader{ RcvData.GetDataArray() };
                const FKLDebugImGuiFeature_NetworkingReceiveDataInput RcvDataInput{ _World, Input.TryGetFeatureContext(), MemoryReader };
                NetworkInterface->ReceiveData(RcvDataInput);
            }
        }
        break;
        default:
            ensureMsgf(false, TEXT("message not handle"));
            break;
        }
    }

    _Reader.SetAtEnd();
}

UPackageMap* FKLDebugImGuiClientManager::GetServerPackageMap() const
{
    UNetDriver* ServerNetDriver = GetWorld().GetNetDriver();
    UNetConnection* NetConnection = ServerNetDriver ? ServerNetDriver->ServerConnection : nullptr;
    if (NetConnection)
    {
        return NetConnection->PackageMap;
    }

    return nullptr;
}
