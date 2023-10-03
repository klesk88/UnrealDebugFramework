#include "Server/KLDebugImGuiServerCacheConnection.h"

//modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Networking/Public/Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "ImGui/Networking/Public/Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Serialization/Archive.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiServerCacheConnection::FKLDebugImGuiServerCacheConnection(FSocket& _ClientSocket)
    : mClientSocket(&_ClientSocket)
{
    mFeaturesPerObject.Reserve(10);
}

FKLDebugImGuiServerCacheConnection::~FKLDebugImGuiServerCacheConnection()
{
    Shutdown();
}

void FKLDebugImGuiServerCacheConnection::Shutdown()
{
    if (mClientSocket)
    {
        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
        check(SocketSubsystem);
        mClientSocket->Close();
        SocketSubsystem->DestroySocket(mClientSocket);
        mClientSocket = nullptr;
    }
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

bool FKLDebugImGuiServerCacheConnection::Recv_SelectableUpdate(const UWorld& _World, const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update)
{
    if (_Update.Server_IsFullyRemoved())
    {
        RemoveObjectFeatures(_Update.Server_GetNetworkID());
        return true;
    }

    FKLDebugImGuiServerObjectFeatures& ServerObjectFeatureData = GetOrAddFeaturesPerObject(_World, _Update.Server_GetNetworkID());
    if (!ServerObjectFeatureData.GetCachedObject())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("KLDebugImGuiNetworkingManager_Server::Rcv_HandleClientFeatureStatusUpdate>> received packet for feature but object does not exist anymore"));
        return true;
    }

    const ENetMode NetMode = _World.GetNetMode();
    for (const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData& NewData : _Update.Server_GetFeaturesData())
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
            ServerObjectFeatureData.AddFeature(Input, FeatureInterface, FeatureRealIndex.GetValue());
        }
        else
        {
            ServerObjectFeatureData.RemoveFeature(FeatureRealIndex.GetValue());
        }
    }

    return true;
}

bool FKLDebugImGuiServerCacheConnection::Recv_UniqueUpdate(const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update)
{
    if (_Update.Server_IsFullyRemoved())
    {
        mUniqueFeatures.Clear();
        return true;
    }

    for (const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData& NewData : _Update.Server_GetFeaturesData())
    {
        const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FeatureRealIndex = GetFeatureIndex(_Container, NewData);
        if (!FeatureRealIndex.IsSet())
        {
            continue;
        }

        if (NewData.Server_IsAdded())
        {
            mUniqueFeatures.AddFeature(FeatureRealIndex.GetValue());
        }
        else
        {
            mUniqueFeatures.RemoveFeature(FeatureRealIndex.GetValue());
        }
    }

    return true;
}

TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> FKLDebugImGuiServerCacheConnection::GetFeatureIndex(const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData& _Data) const
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
    const TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> CurrentFeatureIndex = EngineSubystem->CookedOnly_TryGetFeatureFromName(FeatureData.Server_GetFeatureNameID());
    FeatureIndex = CurrentFeatureIndex;
#endif
}

bool FKLDebugImGuiServerCacheConnection::Rcv_HandleClientFeatureStatusUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FArchive& _Archive)
{
    FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate FeatureStatusUpdate;
    FeatureStatusUpdate.Read(_World, _Archive);

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
        return Recv_UniqueUpdate(Container, FeatureStatusUpdate);
    case EImGuiInterfaceType::COUNT:
        return false;
    }

    return false;
}

void FKLDebugImGuiServerCacheConnection::Write_ConnectionFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive) const
{
    TArray<uint8> Data;
    int32 BytesCount = 0;
    int32 BytesMax = 0;
    _Archive.CountBytes(BytesCount, BytesMax);
    Data.Reserve(BytesMax);

    Write_UniqueFeatures(_World, _FeatureContainer, _Archive, Data);
    Write_ObjectFeatures(_World, _FeatureContainer, _Archive, Data);
}

void FKLDebugImGuiServerCacheConnection::Write_UniqueFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive, TArray<uint8>& _Data) const
{
    const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainer.GetContainer(EImGuiInterfaceType::UNIQUE);

    const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& Feature = mUniqueFeatures.GetFeatures();
    FKLDebugImGuiSubsetFeaturesConstIterator Iterator = Container.GetFeaturesSubsetConstIterator(Feature);

    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
        if (!NetworkInterface)
        {
            ensureMsgf(false, TEXT("NetworkInterface must be valid at this point"));
            continue;
        }

        _Data.Reset();
        //FMemoryWriter Writer{ _Data };
        //const FKLDebugImGuiFeature_NetworkingGatherDataInput GatherDataInput{ _World, EKLDebugImGuiNetworkingEnviroment::Server, _World, nullptr, Writer };
        //if (NetworkInterface->ShouldGatherData(GatherDataInput))
        //{
        //    NetworkInterface->GatherData(GatherDataInput);

        //    /*          FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate DataUpdate{ Feature.GetNetworkID(), FeatureData.GetFeatureIndex(), Data };
        //              DataUpdate.Write(_World, _Archive);*/
        //}
    }
}

void FKLDebugImGuiServerCacheConnection::Write_ObjectFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive, TArray<uint8>& _Data) const
{
    const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainer.GetContainer(EImGuiInterfaceType::SELECTABLE);

    for (const FKLDebugImGuiServerObjectFeatures& Feature : mFeaturesPerObject)
    {
        UObject* OwnerObject = Feature.GetCachedObjectMutable();
        if (!OwnerObject)
        {
            continue;
        }

        const TArray<FKLDebugImGuiServerObjectFeatureData>& FeaturesList = Feature.GetEnableFetures();
        if (FeaturesList.IsEmpty())
        {
            continue;
        }

        for (const FKLDebugImGuiServerObjectFeatureData& FeatureData : FeaturesList)
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeature(FeatureData.GetFeatureIndex());
            const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
            if (!NetworkInterface)
            {
                ensureMsgf(false, TEXT("NetworkInterface must be valid at this point"));
                continue;
            }

            _Data.Reset();
            FMemoryWriter Writer{ _Data };
            const FKLDebugImGuiFeature_NetworkingGatherDataInput GatherDataInput{ _World, EKLDebugImGuiNetworkingEnviroment::Server, *OwnerObject, FeatureData.GetContextMutable(), Writer };
            if (NetworkInterface->ShouldGatherData(GatherDataInput))
            {
                NetworkInterface->GatherData(GatherDataInput);

                FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate DataUpdate{ Feature.GetNetworkID(), FeatureData.GetFeatureIndex(), _Data };
                DataUpdate.Write(_World, _Archive);
            }
        }
    }
}

