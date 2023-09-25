#include "Server/KLDebugImGuiNetworkingCacheConnection.h"

//imgui module
#include "ImGui/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "ImGui/Public/Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"

//engine
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

void FKLDebugImGuiNetworkingCacheConnection::Write_ConnectionFeatures(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FNetBitWriter& _Writer) const
{
    for (const FKLDebugImGuiNetworking_ServerObjectFeatures& Feature : mFeaturesPerObject)
    {
        for (const FKLDebugImGuiNetworking_ServerObjectContainerFeatures& FeatureContainer : Feature.GetContainers())
        {
            const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& Features = FeatureContainer.GetEnableFetures();
            const EContainerType ContainerType = FeatureContainer.GetContainerType();
            if (Features.IsEmpty())
            {
                continue;
            }
        }
    }
}

