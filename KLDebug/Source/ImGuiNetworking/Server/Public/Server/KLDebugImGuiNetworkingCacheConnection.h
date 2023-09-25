#pragma once

#include "Features/KLDebugImGuiNetworking_ServerObjectFeatures.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/RefCounting.h"

class FKLDebugImGuiNetworkManager_Base;
class FNetworkGUID;
class FSocket;
class UWorld;

//based on FEditorDomainSaveServer::FClientConnection
class KLDEBUGIMGUINETWORKINGSERVER_API FKLDebugImGuiNetworkingCacheConnection final : public FRefCountedObject
{
public:
    explicit FKLDebugImGuiNetworkingCacheConnection(FSocket& _ClientSocket);
    ~FKLDebugImGuiNetworkingCacheConnection();

    /** Release resources and indicate this connection should no longer be used. */
    void Shutdown();

    UE_NODISCARD bool IsValid() const;
    UE_NODISCARD FSocket& GetSocket() const;

    UE_NODISCARD FKLDebugImGuiNetworking_ServerObjectFeatures& GetOrAddFeaturesPerObject(const UWorld& _World, const FNetworkGUID& _NetworkID);
    UE_NODISCARD FKLDebugImGuiNetworking_ServerObjectFeatures* TryGetFeaturesPerObjectMutable(const FNetworkGUID& _NetworkID);
    void RemoveObjectFeatures(const FNetworkGUID& _NetworkID);

private:
    TArray<FKLDebugImGuiNetworking_ServerObjectFeatures> mFeaturesPerObject;
    /** The socket to the client process, returned from accept on the server's ListenSocket. */
    FSocket* mClientSocket = nullptr;
};

inline bool FKLDebugImGuiNetworkingCacheConnection::IsValid() const
{
    return mClientSocket != nullptr;
}

inline FSocket& FKLDebugImGuiNetworkingCacheConnection::GetSocket() const
{
    return *mClientSocket;
}

inline FKLDebugImGuiNetworking_ServerObjectFeatures* FKLDebugImGuiNetworkingCacheConnection::TryGetFeaturesPerObjectMutable(const FNetworkGUID& _NetworkID)
{
    FKLDebugImGuiNetworking_ServerObjectFeatures* Feature = mFeaturesPerObject.FindByKey(_NetworkID);
    return Feature;
}