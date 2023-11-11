#include "Server/KLDebugImGuiServerCacheConnection.h"

#include "Server/KLDebugImGuiServerConnectionDefinitions.h"
#include "Subsystem/Engine/KLDebugImGuiServerSubsystem_Engine.h"

//modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Networking/Public/Message/Feature/DataUpdate/KLDebugImGuiNetworkingMessage_FeatureDataUpdate.h"
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "ImGui/Networking/Public/Message/Helpers/KLDebugImGuiNetworkingMessage_Helpers.h"
#include "ImGui/Networking/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingPendingMessage.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Containers/ArrayView.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/Crc.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#if !WITH_EDITOR
#include "Subsystem/KLDebugImGuiServerSubsystem_Engine.h"
#endif

#if !NO_LOGGING
//engine
#include "Containers/UnrealString.h"
#include "UObject/NameTypes.h"
#endif

namespace KL::Debug::Networking::ImGuiServer
{
    void Write_FeatureUpdatePrivate(const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _InterfaceType, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const bool _IsCompressed, const uint32 _CompressTotalSize, const uint32 _UncompressTotalSize, TArray<uint8>& _FeatureData, TArray<uint8>& _Data, FArchive& _Archive)
    {
        int32 MessagesCount = FMath::CeilToInt(static_cast<float>(_FeatureData.Num()) * KL::Debug::ImGui::Networking::Server::FeatureMaxMessageDataSizeInv);
        MessagesCount = FMath::Clamp(MessagesCount, 1, MessagesCount);
        const KL::Debug::ImGui::Networking::Message::MessageID MessageID = MessagesCount > 1 ? KL::Debug::ImGui::Networking::Message::GetNewMessageID() : 0;

        _Data.Reset();
        FMemoryWriter FinalData{ _Data };

        FKLDebugImGuiNetworkingMessage_FeatureDataUpdate Message{ _NetworkID, _InterfaceType, _FeatureInterface.GetFeatureNameID(), _ClientFeatureIndex };
        Message.Serialize(FinalData);
        const uint32 MessageSize = static_cast<uint32>(_Data.Num());
        uint32 MessageOffset = 0;

        const uint32 FinalTotalUncompressSize = _UncompressTotalSize + MessageSize;
        const uint32 FinalTotalCompressSize = _IsCompressed ? _CompressTotalSize + MessageSize : 0;
        const uint32 StartCompressDataOffset = _IsCompressed ? MessageSize : FinalTotalCompressSize + 100;

#if DO_ENSURE
        uint32 EnsureTotalSizeSent = 0;
#endif

        for (int32 i = 0; i < MessagesCount; ++i)
        {
            const int32 StartIndex = i * KL::Debug::ImGui::Networking::Server::FeatureMaxMessageDataSize;
            const int32 EndIndex = FMath::Min((i + 1) * static_cast<int32>(KL::Debug::ImGui::Networking::Server::FeatureMaxMessageDataSize), _FeatureData.Num());
            checkf(EndIndex - StartIndex <= _FeatureData.Num(), TEXT("going out of bounds"));
            const TArrayView<uint8> MessageDataView(&_FeatureData[StartIndex], EndIndex - StartIndex);

            FinalData.Serialize(MessageDataView.GetData(), MessageDataView.Num());

            KL::Debug::ImGuiNetworking::Message::PrepareMessageToSend(
                Message,
                _IsCompressed,
                FinalTotalUncompressSize,
                FinalTotalCompressSize,
                MessageOffset,
                StartCompressDataOffset,
                MessageID,
                _Data,
                _Archive);

            MessageOffset += _Data.Num();

#if DO_ENSURE
            EnsureTotalSizeSent += _Data.Num();
#endif

            _Data.Reset();
            FinalData.Seek(0);
        }

        ensureMsgf(_IsCompressed ? EnsureTotalSizeSent == FinalTotalCompressSize : EnsureTotalSizeSent == FinalTotalUncompressSize, TEXT("we sent more or less bytes then exptected"));
    }

    void Write_FeatureUpdateCommon(const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _InterfaceType, const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const UWorld& _World, UObject* _Owner, FKLDebugImGuiFeatureContext_Base* _Context, TArray<uint8>& _Data, TArray<uint8>& _TempFeatureData, TArray<uint8>& _CompressFeatureData, uint32& _DataCRC, FArchive& _Archive)
    {
        const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = _FeatureInterface.TryGetNetworkInterface();
        if (!NetworkInterface)
        {
            ensureMsgf(false, TEXT("NetworkInterface must be valid at this point"));
            return;
        }

        _TempFeatureData.Reset();

        FMemoryWriter FeatureWriter{ _TempFeatureData };
        const FKLDebugImGuiFeature_NetworkingGatherDataInput GatherDataInput{ _World, EKLDebugImGuiNetworkingEnviroment::Server, _Owner, _Context, FeatureWriter };
        if (!NetworkInterface->ShouldGatherData(GatherDataInput))
        {
            return;
        }

        NetworkInterface->GatherData(GatherDataInput);
        if (_TempFeatureData.IsEmpty())
        {
            return;
        }

        if (NetworkInterface->ShouldVerifyCRCBeforeSendData())
        {
            //based on GameplayDebuggerDataPack::CheckDirtyAndUpdate
            const uint32 NewDataCRC = FCrc::MemCrc32(_TempFeatureData.GetData(), _TempFeatureData.Num());
            if (_DataCRC == NewDataCRC)
            {
                return;
            }

            _DataCRC = NewDataCRC;
        }

        _CompressFeatureData.Reset();
        TArray<uint8>* FeatureBufferToUse = &_TempFeatureData;
        const uint32 UncompressSize = static_cast<uint32>(_TempFeatureData.Num());
        const bool ShouldCompressData = NetworkInterface->ShouldCompressData();
        if (ShouldCompressData)
        {
            KL::Debug::ImGuiNetworking::Message::CompressBuffer(_TempFeatureData, _CompressFeatureData);
            if (_CompressFeatureData.IsEmpty())
            {
                UE_LOG(LogKL_Debug, Error, TEXT("Write_FeatureUpdateCommon>> Compressing data for feature [%s] failed, returned an empty data buffer"),
                    *_FeatureInterface.GetFeatureNameID().ToString());
                return;
            }

            FeatureBufferToUse = &_CompressFeatureData;
            UE_LOG(LogKL_Debug, Display, TEXT("Write_FeatureUpdateCommon>> Compressing feature data [%s]. Original size [%d] compress size [%d]"),
                *_FeatureInterface.GetFeatureNameID().ToString(),
                UncompressSize,
                _CompressFeatureData.Num());
        }

        const uint32 CompressSize = static_cast<uint32>(_CompressFeatureData.Num());
        Write_FeatureUpdatePrivate(_FeatureInterface, 
            _NetworkID, 
            _InterfaceType, 
            _ClientFeatureIndex, 
            ShouldCompressData, 
            CompressSize, 
            UncompressSize, 
            *FeatureBufferToUse, 
            _Data, 
            _Archive);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

FKLDebugImGuiServerCacheConnection::FKLDebugImGuiServerCacheConnection()
{
    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

    mFeaturesPerObject.Reserve(10);
    mTempData.Reserve(Settings.Server_GetConnectionTempDataSize());
    mTempFeatureData.Reserve(Settings.Server_GetConnectionTempDataSize());
    mTempCompressedData.Reserve(Settings.Server_GetConnectionTempCompressDataSize());
}

FKLDebugImGuiServerObjectFeatures& FKLDebugImGuiServerCacheConnection::GetOrAddFeaturesPerObject(const UWorld& _World, const FNetworkGUID& _NetworkID)
{
    ensureMsgf(_NetworkID.IsValid(), TEXT("networkd id must be valid"));

    FKLDebugImGuiServerObjectFeatures* Feature = mFeaturesPerObject.FindByKey(_NetworkID);
    if (!Feature)
    {
        Feature = &mFeaturesPerObject.Emplace_GetRef(_World, _NetworkID);
    }

    return *Feature;
}

void FKLDebugImGuiServerCacheConnection::RemoveObjectFeatures(const FNetworkGUID& _NetworkID)
{
    const int32 Index = mFeaturesPerObject.IndexOfByKey(_NetworkID);
    if (Index != INDEX_NONE)
    {
        mFeaturesPerObject.RemoveAtSwap(Index, 1, false);
    }
}

TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FKLDebugImGuiServerCacheConnection::GetFeatureIndex(const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusData& _Data) const
{
    if (_Container.IsValidFeatureIndex(_Data.Server_GetFeatureIndex(), _Data.Server_GetFeatureNameID()))
    {
        return _Data.Server_GetFeatureIndex();
    }
    
    //client and server can be out of sync in a deployed environment (different version). So try to see if the server has the correct index
    //for this feature

#if WITH_EDITOR
    //in editor mode we cant be possibly out of sync. There is a bug somewhere
    ensureMsgf(false, TEXT("we could not find the feature client and server are out of sync not possible on editor builds. Dropping connection to client"));
    return TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex>();
#else
    const UKLDebugImGuiServerSubsystem_Engine* EngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGet();
    if (EngineSubsystem)
    {
        const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> CurrentFeatureIndex = EngineSubsystem->CookedOnly_TryGetFeatureFromName(_Data.Server_GetFeatureNameID());
        return CurrentFeatureIndex;
    }

    return TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex>();
#endif
}

void FKLDebugImGuiServerCacheConnection::ReadData(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, TArray<FKLDebugImGuiNetworkingPendingMessage>& _MessagesReceived)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiServerCacheConnection_ReadData);

    for (const FKLDebugImGuiNetworkingPendingMessage& PendingMessage : _MessagesReceived)
    {
        const EKLDebugNetworkMessageTypes MessageType = PendingMessage.GetMesageType();
        FMemoryReader MessageDataReader{ PendingMessage.GetMessageData() };
        switch (MessageType)
        {
        case EKLDebugNetworkMessageTypes::Client_FeatureStatusUpdate:
        {
            static_cast<void>(Rcv_HandleClientFeatureStatusUpdate(_FeatureContainer, _World, MessageDataReader));
            break;
        }
        default:
            ensureMsgf(false, TEXT("message not handle"));
            break;
        }
    }

    _MessagesReceived.Reset();
}

bool FKLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureStatusUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FArchive& _Archive)
{
    FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate FeatureStatusUpdate{ _Archive };
    const ENetMode NetMode = _World.GetNetMode();
    if (FeatureStatusUpdate.Server_IsFullyRemoved())
    {
        RemoveObjectFeatures(FeatureStatusUpdate.Server_GetNetworkID());
        return true;
    }

    if (FeatureStatusUpdate.Server_GetContainerType() == EImGuiInterfaceType::COUNT)
    {
        return false;
    }

    const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainerManager.GetContainer(FeatureStatusUpdate.Server_GetContainerType());
    switch (FeatureStatusUpdate.Server_GetContainerType())
    {
    case EImGuiInterfaceType::SELECTABLE:
        return Recv_SelectableUpdate(_World, Container, FeatureStatusUpdate);
    case EImGuiInterfaceType::UNIQUE:
        return Recv_UniqueUpdate(_World, Container, FeatureStatusUpdate);
    case EImGuiInterfaceType::COUNT:
        UE_LOG(LogKL_Debug, Error, TEXT("KLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureStatusUpdate>> invalid imgui type disconnecting"));
        return false;
    }

    return false;
}


bool FKLDebugImGuiServerCacheConnection::Recv_SelectableUpdate(const UWorld& _World, const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update)
{
    if (_Update.Server_IsFullyRemoved())
    {
        RemoveObjectFeatures(_Update.Server_GetNetworkID());
        return true;
    }

    if (!_Update.Server_GetNetworkID().IsValid())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiServerCacheConnection::Recv_SelectableUpdate>> received packet for feature network id is not valid. should never happen"));
        return false;
    }

    FKLDebugImGuiServerObjectFeatures& ServerObjectFeatureData = GetOrAddFeaturesPerObject(_World, _Update.Server_GetNetworkID());
    if (!ServerObjectFeatureData.GetCachedObject())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiServerCacheConnection::Recv_SelectableUpdate>> received packet for feature but object does not exist anymore"));
        return true;
    }

    const ENetMode NetMode = _World.GetNetMode();
    for (const FKLDebugImGuiNetworkingMessage_FeatureStatusData& NewData : _Update.Server_GetFeaturesData())
    {
        const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FeatureRealIndex = GetFeatureIndex(_Container, NewData);
        if (!FeatureRealIndex.IsSet())
        {
            continue;
        }

        if (NewData.Server_IsAdded())
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = _Container.GetFeature(FeatureRealIndex.GetValue());
            const FKLDebugImGuiFeatureContextInput Input{ NetMode, *ServerObjectFeatureData.GetCachedObject() };
            ServerObjectFeatureData.AddFeature(Input, FeatureInterface, NewData.Server_GetFeatureIndex(), FeatureRealIndex.GetValue());
        }
        else
        {
            ServerObjectFeatureData.RemoveFeature(FeatureRealIndex.GetValue());
        }
    }

    return true;
}

bool FKLDebugImGuiServerCacheConnection::Recv_UniqueUpdate(const UWorld& _World, const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update)
{
    if (_Update.Server_IsFullyRemoved())
    {
        mUniqueFeatures.Clear();
        return true;
    }

    const ENetMode NetMode = _World.GetNetMode();
    for (const FKLDebugImGuiNetworkingMessage_FeatureStatusData& NewData : _Update.Server_GetFeaturesData())
    {
        const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FeatureRealIndex = GetFeatureIndex(_Container, NewData);
        if (!FeatureRealIndex.IsSet())
        {
            continue;
        }

        if (NewData.Server_IsAdded())
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = _Container.GetFeature(FeatureRealIndex.GetValue());
            const FKLDebugImGuiFeatureContextInput Input{ NetMode, _World };
            mUniqueFeatures.AddFeature(Input, FeatureInterface, NewData.Server_GetFeatureIndex(), FeatureRealIndex.GetValue());
        }
        else
        {
            mUniqueFeatures.RemoveFeature(FeatureRealIndex.GetValue());
        }
    }

    return true;
}

void FKLDebugImGuiServerCacheConnection::WriteData(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiServerCacheConnection_WriteData);

    Write_ConnectionFeatures(_World, _FeatureContainer, _Archive);
}

void FKLDebugImGuiServerCacheConnection::Write_ConnectionFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive)
{
    const int32 InitializeTempSize = mTempData.Max();

    Write_UniqueFeatures(_World, _FeatureContainer, _Archive);
    Write_ObjectFeatures(_World, _FeatureContainer, _Archive);

    mTempData.SetNum(InitializeTempSize);
    mTempCompressedData.SetNum(InitializeTempSize);
}

void FKLDebugImGuiServerCacheConnection::Write_UniqueFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive)
{
    const EImGuiInterfaceType InterfaceType = EImGuiInterfaceType::UNIQUE;
    const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainer.GetContainer(EImGuiInterfaceType::UNIQUE);

    TArray<FKLDebugImGuiServerUniqueFeatureData>& UniqueFeatures = mUniqueFeatures.GetFeaturesMutable();
    FNetworkGUID FakeGuid;

    for (FKLDebugImGuiServerUniqueFeatureData& FeatureData : UniqueFeatures)
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeature(FeatureData.GetServerFeatureIndex());
        const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
        if (!NetworkInterface)
        {
            ensureMsgf(false, TEXT("NetworkInterface must be valid at this point"));
            continue;
        }

        uint32 CRC = FeatureData.GetCRC();
        KL::Debug::Networking::ImGuiServer::Write_FeatureUpdateCommon(
            FeatureInterface,
            FakeGuid,
            InterfaceType,
            FeatureInterface.GetFeatureNameID(),
            FeatureData.GetServerFeatureIndex(),
            _World,
            nullptr,
            FeatureData.GetFeatureContextMutable(),
            mTempData,
            mTempFeatureData,
            mTempCompressedData,
            CRC,
            _Archive);

        FeatureData.SetCRC(CRC);
    }
}

void FKLDebugImGuiServerCacheConnection::Write_ObjectFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive)
{
    const EImGuiInterfaceType InterfaceType = EImGuiInterfaceType::SELECTABLE;
    const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainer.GetContainer(InterfaceType);

    for (FKLDebugImGuiServerObjectFeatures& Feature : mFeaturesPerObject)
    {
        UObject* OwnerObject = Feature.GetCachedObjectMutable();
        if (!OwnerObject)
        {
            continue;
        }

        TArray<FKLDebugImGuiServerObjectFeatureData>& FeaturesList = Feature.GetEnableFeturesMutable();
        if (FeaturesList.IsEmpty())
        {
            continue;
        }

        for (FKLDebugImGuiServerObjectFeatureData& FeatureData : FeaturesList)
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeature(FeatureData.GetServerFeatureIndex());
            uint32 CRC = FeatureData.GetLastSentCRC();
            KL::Debug::Networking::ImGuiServer::Write_FeatureUpdateCommon(
                FeatureInterface,
                Feature.GetNetworkID(),
                InterfaceType,
                FeatureInterface.GetFeatureNameID(),
                FeatureData.GetClientFeatureIndex(),
                _World,
                OwnerObject,
                FeatureData.GetContextMutable(),
                mTempData,
                mTempFeatureData,
                mTempCompressedData,
                CRC,
                _Archive);

            FeatureData.SetLastSendCRC(CRC);
        }
    }
}

