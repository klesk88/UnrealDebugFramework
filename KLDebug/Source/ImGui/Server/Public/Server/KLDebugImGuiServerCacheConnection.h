#pragma once

#include "Features/KLDebugImGuiServerObjectFeatures.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/RefCounting.h"

class FArchive;
class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiNetworkManager_Base;
class FNetworkGUID;
class FSocket;
class UWorld;

//based on FEditorDomainSaveServer::FClientConnection
class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerCacheConnection final : public FRefCountedObject
{
public:
    explicit FKLDebugImGuiServerCacheConnection(FSocket& _ClientSocket);
    ~FKLDebugImGuiServerCacheConnection();

    /** Release resources and indicate this connection should no longer be used. */
    void Shutdown();

    UE_NODISCARD bool IsValid() const;
    UE_NODISCARD FSocket& GetSocket() const;

    UE_NODISCARD FKLDebugImGuiServerObjectFeatures& GetOrAddFeaturesPerObject(const UWorld& _World, const FNetworkGUID& _NetworkID);
    UE_NODISCARD FKLDebugImGuiServerObjectFeatures* TryGetFeaturesPerObjectMutable(const FNetworkGUID& _NetworkID);
    void RemoveObjectFeatures(const FNetworkGUID& _NetworkID);

    void Write_ConnectionFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive) const;

private:
    TArray<FKLDebugImGuiServerObjectFeatures> mFeaturesPerObject;
    /** The socket to the client process, returned from accept on the server's ListenSocket. */
    FSocket* mClientSocket = nullptr;
};

inline bool FKLDebugImGuiServerCacheConnection::IsValid() const
{
    return mClientSocket != nullptr;
}

inline FSocket& FKLDebugImGuiServerCacheConnection::GetSocket() const
{
    return *mClientSocket;
}

inline FKLDebugImGuiServerObjectFeatures* FKLDebugImGuiServerCacheConnection::TryGetFeaturesPerObjectMutable(const FNetworkGUID& _NetworkID)
{
    FKLDebugImGuiServerObjectFeatures* Feature = mFeaturesPerObject.FindByKey(_NetworkID);
    return Feature;
}
