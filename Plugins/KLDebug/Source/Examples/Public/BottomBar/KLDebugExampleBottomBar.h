// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Context/KLDebugContextInterface.h"
#include "User/Framework/Public/Networking/Window/KLDebugNetworkingWindowInterface.h"
#include "User/Framework/Public/Window/BottomBar/KLDebugBottomBarInterface.h"

class KLDEBUGEXAMPLES_API FDebugExampleBottombarClientContext final : public IKLDebugContextInterface
{
    KL_DEBUG_DERIVED_CONTEXT(FDebugExampleBottombarClientContext, IKLDebugContextInterface)

public:
    int32 mSyncedCurrentTileX = 0;
    int32 mSyncedCurrentTileY = 0;
    int32 mRequestServerTileCount = 0;
    bool mHasRequestUpdateToServer = false;
};

class KLDEBUGEXAMPLES_API FDebugExampleBottombarServerContext final : public IKLDebugContextInterface
{
    KL_DEBUG_DERIVED_CONTEXT(FDebugExampleBottombarServerContext, IKLDebugContextInterface)

public:
    int32 mSyncedCurrentTileX = 0;
    int32 mSyncedCurrentTileY = 0;
};

class KLDEBUGEXAMPLES_API FKLDebugExampleBottomBar final : public IKLDebugBottomBarInterface, public IKLDebugNetworkingWindowInterface
{
    KL_DEBUG_DERIVED_BOTTOMBAR(FKLDebugExampleBottomBar, IKLDebugBottomBarInterface)
    KL_DEBUG_NETWORK_INTERFACE_WINDOW(FKLDebugExampleBottomBar)

public:
    // IKLDebugBottomBarInterface
    const FString& GetFriendlyName() const final;
    void Draw(const FKLDebugBottomBarDrawInput& _Input) const final;
    TUniquePtr<IKLDebugContextInterface> GetContext(const FKLDebugContextGetterInput& _Input) const final;
    // IKLDebugBottomBarInterface

    // IKLDebugNetworkingWindowInterface
    void Server_Tick(const FKLDebugNetworkingWindowServerTickInput& _Input) final;
    void Server_FeatureUpdate(const FKLDebugNetworkingWindowRequestUpdateInput& _Input) const final;

    void Client_ReceiveData(const FKLDebugNetworkingWindowReceiveDataInput& _Input) final;
    void Client_Tick(FKLDebugNetworkingWindowClientTickInput& _Input) final;
    // IKLDebugNetworkingWindowInterface
};
