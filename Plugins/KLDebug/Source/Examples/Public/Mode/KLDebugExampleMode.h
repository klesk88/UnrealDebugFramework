// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Context/KLDebugContextInterface.h"
#include "User/Framework/Public/Mode/KLDebugModeInterface.h"
#include "User/Framework/Public/Networking/Window/KLDebugNetworkingWindowInterface.h"

// engine
#include "Containers/Array.h"

class KLDEBUGEXAMPLES_API FDebugExampleModeClientContext final : public IKLDebugContextInterface
{
    KL_DEBUG_DERIVED_CONTEXT(FDebugExampleModeClientContext, IKLDebugContextInterface)

public:
    int32 mSyncedCurrentTileX = 0;
    int32 mSyncedCurrentTileY = 0;
    int32 mRequestServerTileCount = 0;
    bool mHasRequestUpdateToServer = false;
};

class KLDEBUGEXAMPLES_API FDebugExampleModeServerContext final : public IKLDebugContextInterface, public IKLDebugNetworkingWindowInterface
{
    KL_DEBUG_DERIVED_CONTEXT(FDebugExampleModeServerContext, IKLDebugContextInterface)
    KL_DEBUG_NETWORK_INTERFACE_WINDOW(FKLDebugExampleBottomBar)

public:
    int32 mSyncedCurrentTileX = 0;
    int32 mSyncedCurrentTileY = 0;
};

class KLDEBUGEXAMPLES_API FKLDebugExampleMode final : public IKLDebugModeInterface, public IKLDebugNetworkingWindowInterface
{
    KL_DEBUG_DERIVED_MODE(FKLDebugExampleMode, IKLDebugModeInterface)
    KL_DEBUG_NETWORK_INTERFACE_WINDOW(FKLDebugExampleMode)

public:
    // IKLDebugModeInterface
    void OnSelect(const UWorld& _World);

    const FString& GetFriendlyName() const;
    void DrawImGui(const FKLDebugModeDrawImGuiInput& _Input) final;
    void DrawCanvas(FKLDebugModeDrawCanvasInput& _Input) const final;
    TUniquePtr<IKLDebugContextInterface> GetContext(const FKLDebugContextGetterInput& _Input) const final;
    // IKLDebugModeInterface

    // IKLDebugNetworkingWindowInterface
    void Server_Tick(const FKLDebugNetworkingWindowServerTickInput& _Input) final;
    void Server_FeatureUpdate(const FKLDebugNetworkingWindowRequestUpdateInput& _Input) const final;

    void Client_ReceiveData(const FKLDebugNetworkingWindowReceiveDataInput& _Input) final;
    void Client_Tick(FKLDebugNetworkingWindowClientTickInput& _Input) final;
    // IKLDebugNetworkingWindowInterface

private:
    TArray<float> ImPlotExampleXFirst;
    TArray<float> ImPlotExampleYFirst;
    TArray<double> ImPlotExampleXSecond;
    TArray<double> ImPlotExampleYSecond;
    float mPlotXSize = 300.f;
    float mPlotYSize = 200.f;
};
