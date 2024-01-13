// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiServerCacheConnection.h"

#include "Subsystem/Engine/KLDebugImGuiServerSubsystem_Engine.h"

// modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Networking/Public/Message/Feature/DataUpdate/KLDebugImGuiNetworkingMessage_FeatureDataUpdate.h"
#include "ImGui/Networking/Public/Message/Feature/RequestUpdate/KLDebugImGuiNetworkingMessage_FeatureRequestUpdate.h"
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "ImGui/Networking/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Selectable/Input/KLDebugImGuiFeatureContextInput_Selectable.h"
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "ImGui/User/Public/Feature/Interface/Unique/Input/KLDebugImGuiFeatureContextInput_Unique.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingTickInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugNetworkingFeature_RequestUpdateInput.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingSplittedMessage.h"

// engine
#include "Containers/ArrayView.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/Crc.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#if !NO_LOGGING
// engine
#include "Containers/UnrealString.h"
#include "UObject/NameTypes.h"
#endif

namespace KL::Debug::Networking::ImGuiServer
{
    /////////////////////////////////////////////////////////////////////////
    /// private

    void Write_FeatureUpdateCommon(IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _InterfaceType, const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const UWorld& _World, UObject* _Owner, FKLDebugImGuiFeatureContext_Base* _Context, TArray<uint8>& _Data, TArray<uint8>& _TempFeatureData, TArray<uint8>& _CompressFeatureData, uint32& _DataCRC, FArchive& _Archive)
    {
        IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = _FeatureInterface.TryGetNetworkInterfaceMutable();
        checkf(NetworkInterface != nullptr, TEXT("we should have check the interface in CanAddFeatureToTickList"));

        _TempFeatureData.Reset();

        FMemoryWriter FeatureWriter{ _TempFeatureData };
        const FKLDebugImGuiFeature_NetworkingGatherDataInput GatherDataInput{ _World, _Owner, _Context, FeatureWriter };
        if (!NetworkInterface->Server_ShouldTick(GatherDataInput))
        {
            return;
        }

        NetworkInterface->Server_Tick(GatherDataInput);
        if (_TempFeatureData.IsEmpty())
        {
            return;
        }

        if (NetworkInterface->Server_ShouldVerifyCRCBeforeSendData())
        {
            // based on GameplayDebuggerDataPack::CheckDirtyAndUpdate
            const uint32 NewDataCRC = FCrc::MemCrc32(_TempFeatureData.GetData(), _TempFeatureData.Num());
            if (_DataCRC == NewDataCRC)
            {
                return;
            }

            _DataCRC = NewDataCRC;
        }

        _CompressFeatureData.Reset();
        _Data.Reset();
        FMemoryWriter FinalData{ _Data };
        FKLDebugImGuiNetworkingMessage_FeatureDataUpdate Message{ _NetworkID, _InterfaceType, _FeatureInterface.GetFeatureNameID(), _ClientFeatureIndex };
        Message.Serialize(FinalData);
        FinalData.Serialize(_TempFeatureData.GetData(), _TempFeatureData.Num());

        KL::Debug::Networking::Message::PrepareMessageToSend_SplitIfRequired(Message, _Data, _CompressFeatureData, _Archive);
    }

}    // namespace KL::Debug::Networking::ImGuiServer

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

TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FKLDebugImGuiServerCacheConnection::GetFeatureIndex(const FKLDebugImGuiFeatureContainerBase& _Container, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID) const
{
    if (_Container.IsValidFeatureIndex(_FeatureIndex, _FeatureNameID))
    {
        return _FeatureIndex;
    }

    // client and server can be out of sync in a deployed environment (different version). So try to see if the server has the correct index
    // for this feature

#if WITH_EDITOR
    // in editor mode we cant be possibly out of sync. There is a bug somewhere
    ensureMsgf(false, TEXT("we could not find the feature client and server are out of sync not possible on editor builds. Dropping connection to client"));
    return TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex>();
#else
    const UKLDebugImGuiServerSubsystem_Engine* EngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGet();
    if (EngineSubsystem)
    {
        const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> CurrentFeatureIndex = EngineSubsystem->CookedOnly_TryGetFeatureFromName(_FeatureNameID);
        return CurrentFeatureIndex;
    }

    return TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex>();
#endif
}

void FKLDebugImGuiServerCacheConnection::GameThreadTick(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _ArchiveWriter)
{
    const int32 InitializeTempSize = mTempData.Max();

    GameThread_TickPendingMessages(_World, _FeatureContainer, _ArchiveWriter);
    GameThread_TickFeatures(_World, _FeatureContainer, _ArchiveWriter);

    mTempData.SetNum(InitializeTempSize);
    mTempCompressedData.SetNum(InitializeTempSize);
}

void FKLDebugImGuiServerCacheConnection::GameThread_TickPendingMessages(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _ArchiveWriter)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiServerCacheConnection_ReadData);

    for (const FKLDebugNetworkingPendingMessage& PendingMessage : mPendingMessages)
    {
        if (PendingMessage.GetMessageEnumType() != static_cast<uint16>(EKLDebugImGuiNetworkMessageTypes::ImGuiMessage))
        {
            UE_LOG(LogKLDebug_Networking, Error, TEXT("received message of wrong type [%d]"), static_cast<int32>(PendingMessage.GetMessageEnumType()));
            continue;
        }

        const EKLDebugImGuiNetworkMessage MessageType = static_cast<EKLDebugImGuiNetworkMessage>(PendingMessage.GetMessageType());
        FMemoryReader MessageDataReader{ PendingMessage.GetMessageData() };
        switch (MessageType)
        {
        case EKLDebugImGuiNetworkMessage::Client_FeatureStatusUpdate:
        {
            static_cast<void>(Rcv_HandleClientFeatureStatusUpdate(_FeatureContainer, _World, MessageDataReader));
            break;
        }
        case EKLDebugImGuiNetworkMessage::Client_FeatureRequestUpdate:
        {
            FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate FeatureRequestUpdate{ MessageDataReader };
            static_cast<void>(Rcv_HandleClientFeatureRequestUpdate(_FeatureContainer, _World, FeatureRequestUpdate, _ArchiveWriter));
            break;
        }
        default:
            ensureMsgf(false, TEXT("message not handle"));
            break;
        }
    }

    mPendingMessages.Reset();
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
        UE_LOG(LogKLDebug_Networking, Error, TEXT("KLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureStatusUpdate>> invalid imgui type disconnecting"));
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
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiServerCacheConnection::Recv_SelectableUpdate>> received packet for feature network id is not valid. should never happen"));
        return false;
    }

    FKLDebugImGuiServerObjectFeatures& ServerObjectFeatureData = GetOrAddFeaturesPerObject(_World, _Update.Server_GetNetworkID());
    if (!ServerObjectFeatureData.GetCachedObject())
    {
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiServerCacheConnection::Recv_SelectableUpdate>> received packet for feature but object does not exist anymore"));
        return true;
    }

    const ENetMode NetMode = _World.GetNetMode();
    for (const FKLDebugImGuiNetworkingMessage_FeatureStatusData& NewData : _Update.Server_GetFeaturesData())
    {
        const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FeatureRealIndex = GetFeatureIndex(_Container, NewData.Server_GetFeatureIndex(), NewData.Server_GetFeatureNameID());
        if (!FeatureRealIndex.IsSet())
        {
            continue;
        }

        if (NewData.Server_IsAdded())
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = _Container.GetFeature(FeatureRealIndex.GetValue());
            if (!CanAddFeatureToTickList(FeatureInterface))
            {
                continue;
            }

            const FKLDebugImGuiFeatureContextInput_Selectable Input{ NetMode, *ServerObjectFeatureData.GetCachedObject() };
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
        const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FeatureRealIndex = GetFeatureIndex(_Container, NewData.Server_GetFeatureIndex(), NewData.Server_GetFeatureNameID());
        if (!FeatureRealIndex.IsSet())
        {
            continue;
        }

        if (NewData.Server_IsAdded())
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = _Container.GetFeature(FeatureRealIndex.GetValue());
            if (!CanAddFeatureToTickList(FeatureInterface))
            {
                continue;
            }

            const FKLDebugImGuiFeatureContextInput_Unique Input{ NetMode, _World };
            mUniqueFeatures.AddFeature(Input, FeatureInterface, NewData.Server_GetFeatureIndex(), FeatureRealIndex.GetValue());
        }
        else
        {
            mUniqueFeatures.RemoveFeature(FeatureRealIndex.GetValue());
        }
    }

    return true;
}

bool FKLDebugImGuiServerCacheConnection::CanAddFeatureToTickList(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const
{
    const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = _Feature.TryGetNetworkInterface();
    if (!NetworkInterface || !NetworkInterface->RequireServerTick())
    {
        UE_LOG(LogKLDebug_Networking, Error, TEXT("FKLDebugImGuiServerCacheConnection::CanAddFeatureToTickList>> feature [%s] doesnt support server tick. Will not be added"), *_Feature.GetFeatureNameID().ToString());
        return false;
    }

    return true;
}

bool FKLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureRequestUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate& _Message, FArchive& _ArchiveWriter)
{
    const ENetMode NetMode = _World.GetNetMode();
    if (_Message.Server_GetContainerType() == EImGuiInterfaceType::COUNT)
    {
        return true;
    }

    const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainerManager.GetContainer(_Message.Server_GetContainerType());
    const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FeatureRealIndex = GetFeatureIndex(Container, _Message.Server_GetFeatureIndex(), _Message.Server_GetFeatureNameID());
    if (!FeatureRealIndex.IsSet())
    {
        UE_LOG(LogKLDebug_Networking, Warning, TEXT("KLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureRequestUpdate>> feature [%s] could not be found"), *_Message.Server_GetFeatureNameID().ToString());
        return false;
    }

    FNetworkGUID NetworkID;
    mTempFeatureData.Reset();
    FMemoryWriter TempFeatureWriter{ mTempFeatureData };
    FMemoryReader MessageDataReader{ _Message.Server_GetBufferDataMutable() };
    switch (_Message.Server_GetContainerType())
    {
    case EImGuiInterfaceType::SELECTABLE:
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeature(FeatureRealIndex.GetValue());
        if (const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface())
        {
            NetworkID = _Message.Server_GetFeatureObject();
        }

        break;
    }
    case EImGuiInterfaceType::UNIQUE:
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeature(FeatureRealIndex.GetValue());
        if (const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface())
        {
            FKLDebugNetworkingFeature_RequestUpdateInput Input{ _World, MessageDataReader, TempFeatureWriter };
            NetworkInterface->Server_FeatureUpdate(Input);
        }

        break;
    }
    case EImGuiInterfaceType::COUNT:
        UE_LOG(LogKLDebug_Networking, Error, TEXT("KLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureRequestUpdate>> invalid imgui type"));
        return false;
    }

    UE_CLOG(mTempFeatureData.IsEmpty(), LogKLDebug_Networking, Warning, TEXT("KLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureRequestUpdate>> feature [%s] could not found network interface"), *_Message.Server_GetFeatureNameID().ToString());

    if (mTempFeatureData.IsEmpty())
    {
        return false;
    }

    mTempCompressedData.Reset();
    mTempData.Reset();
    FMemoryWriter FinalData{ mTempData };
    FKLDebugImGuiNetworkingMessage_FeatureDataUpdate Message{ NetworkID, _Message.Server_GetContainerType(), _Message.Server_GetFeatureNameID(), _Message.Server_GetFeatureIndex() };
    Message.Serialize(FinalData);
    FinalData.Serialize(mTempFeatureData.GetData(), mTempFeatureData.Num());
    KL::Debug::Networking::Message::PrepareMessageToSend_SplitIfRequired(Message, mTempData, mTempCompressedData, _ArchiveWriter);

    return false;
}

void FKLDebugImGuiServerCacheConnection::GameThread_TickFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiServerCacheConnection_GameThread_TickFeatures);

    GameThead_TickUniqueFeatures(_World, _FeatureContainer, _Archive);
    GameThread_TickObjectFeatures(_World, _FeatureContainer, _Archive);
}

void FKLDebugImGuiServerCacheConnection::GameThead_TickUniqueFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive)
{
    const EImGuiInterfaceType InterfaceType = EImGuiInterfaceType::UNIQUE;
    FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainer.GetContainerMutable(EImGuiInterfaceType::UNIQUE);

    TArray<FKLDebugImGuiServerUniqueFeatureData>& UniqueFeatures = mUniqueFeatures.GetFeaturesMutable();
    FNetworkGUID FakeGuid;

    for (FKLDebugImGuiServerUniqueFeatureData& FeatureData : UniqueFeatures)
    {
        IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeatureMutable(FeatureData.GetServerFeatureIndex());
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

void FKLDebugImGuiServerCacheConnection::GameThread_TickObjectFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive)
{
    const EImGuiInterfaceType InterfaceType = EImGuiInterfaceType::SELECTABLE;
    FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainer.GetContainerMutable(InterfaceType);

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
            IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeatureMutable(FeatureData.GetServerFeatureIndex());
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
