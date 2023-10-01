#include "Server/KLDebugImGuiNetworkingCacheConnection.h"

//modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "ImGuiNetworking/Runtime/Public/Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate.h"

//engine
#include "Serialization/Archive.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiNetworkingCacheConnection::FKLDebugImGuiNetworkingCacheConnection(FSocket& _ClientSocket)
    : mClientSocket(&_ClientSocket)
{
    mFeaturesPerObject.Reserve(10);
}

FKLDebugImGuiNetworkingCacheConnection::~FKLDebugImGuiNetworkingCacheConnection()
{
    Shutdown();
}

void FKLDebugImGuiNetworkingCacheConnection::Shutdown()
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

FKLDebugImGuiNetworking_ServerObjectFeatures& FKLDebugImGuiNetworkingCacheConnection::GetOrAddFeaturesPerObject(const UWorld& _World, const FNetworkGUID& _NetworkID)
{
    ensureMsgf(_NetworkID.IsValid(), TEXT("networkd id must be valid"));

    FKLDebugImGuiNetworking_ServerObjectFeatures* Feature = mFeaturesPerObject.FindByKey(_NetworkID);
    if (!Feature)
    {
        Feature = &mFeaturesPerObject.Emplace_GetRef(_World, _NetworkID);
    }

    return *Feature;
}

void FKLDebugImGuiNetworkingCacheConnection::RemoveObjectFeatures(const FNetworkGUID& _NetworkID)
{
    const int32 Index = mFeaturesPerObject.IndexOfByKey(_NetworkID);
    if (Index != INDEX_NONE)
    {
        mFeaturesPerObject.RemoveAtSwap(Index, 1, false);
    }
}

void FKLDebugImGuiNetworkingCacheConnection::Write_ConnectionFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive) const
{
    TArray<uint8> Data;
    int32 BytesCount = 0;
    int32 BytesMax = 0;
    _Archive.CountBytes(BytesCount, BytesMax);
    Data.Reserve(BytesMax);

    for (const FKLDebugImGuiNetworking_ServerObjectFeatures& Feature : mFeaturesPerObject)
    {
        UObject* OwnerObject = Feature.GetCachedObjectMutable();
        if (!OwnerObject)
        {
            continue;
        }

        for (const FKLDebugImGuiNetworking_ServerObjectContainerFeatures& FeatureContainer : Feature.GetContainers())
        {
            const TArray<FKLDebugImGuiNetworking_ServerObjectFeatureData>& FeaturesList = FeatureContainer.GetEnableFetures();
            const EContainerType ContainerType = FeatureContainer.GetContainerType();
            if (FeaturesList.IsEmpty())
            {
                continue;
            }

            const FKLDebugImGuiFeatureContainerBase& Container = _FeatureContainer.GetContainer(ContainerType);
            for (const FKLDebugImGuiNetworking_ServerObjectFeatureData& FeatureData : FeaturesList)
            {
                const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = Container.GetFeature(FeatureData.GetFeatureIndex());
                const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
                if (!NetworkInterface)
                {
                    ensureMsgf(false, TEXT("NetworkInterface must be valid at this point"));
                    continue;
                }

                Data.Reset();
                FMemoryWriter Writer{ Data };
                const FKLDebugImGuiFeature_NetworkingGatherDataInput GatherDataInput{ _World, EKLDebugImGuiNetworkingEnviroment::Server, *OwnerObject, FeatureData.GetContextMutable(), Writer };
                if (NetworkInterface->ShouldGatherData(GatherDataInput))
                {
                    NetworkInterface->GatherData(GatherDataInput);

                    FKLDebugImGuiNetworkingMessage_SelectableObjectFeatureDataUpdate DataUpdate{ Feature.GetNetworkID(), FeatureData.GetFeatureIndex(), Data };
                    DataUpdate.Write(_World, _Archive);
                }
            }
        }
    }
}

