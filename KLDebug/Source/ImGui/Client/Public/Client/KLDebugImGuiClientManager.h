#pragma once

//modules
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"

//engine
#include "Containers/Array.h"
#include "HAL/Platform.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FArchive;
class FKLDebugImGuiClientData;
class FKLDebugImGuiNetworkingPendingMessage;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientManager
{
public:
    void Init();

    void GameThread_TickReadData(FKLDebugImGuiClientData& _ClientData, TArray<FKLDebugImGuiNetworkingPendingMessage>& _NewData);

    void Parallel_TickWriteData(FArchive& _Writer);

private:
    void GameThread_CopyPendingMessages(FKLDebugImGuiClientData& _ClientData);
    void GameThread_ReadMessages(const UWorld& _World, TArray<FKLDebugImGuiNetworkingPendingMessage>& _NewData);

    void Parallel_WritePendingFeaturesStatusUpdate(TArray<uint8>& _TempData, FArchive& _Archive);

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TArray<uint8> mTempWriteDataBuffer;
};
