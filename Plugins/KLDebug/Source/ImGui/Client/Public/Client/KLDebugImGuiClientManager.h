// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Message/Feature/RequestUpdate/KLDebugImGuiNetworkingMessage_FeatureRequestUpdate.h"
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingMessage.h"

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FArchive;
class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTickInput;
class FKLDebugNetworkingPendingMessage;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientManager final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientManager(const UWorld& _World);

    void Init();

    void AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage);

    void GameThread_Tick(const UWorld& _World);
    void Parallel_TickWriteData(FArchive& _Writer);

    UE_NODISCARD bool RequiresGameThreadTick() const;

private:
    void GameThread_ReadMessages(const UWorld& _World);

    void OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);
    void OnFeaturesTick(const FKLDebugImGuiFeaturesTickInput& _Input);

    void TickUniqueFeatures(const FKLDebugImGuiFeaturesTickInput& _Input);
    void TickObjectFeatures(const FKLDebugImGuiFeaturesTickInput& _Input);

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mParallelPendingFeaturesStatusUpdates;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TArray<FKLDebugNetworkingPendingMessage> mPendingMessages;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate> mParallelPendingFeatureRequestUpdate;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate> mPendingFeatureRequestUpdate;
    TArray<uint8> mTempWriteDataBuffer;
    TArray<uint8> mTempTickBuffer;
    TMap<FObjectKey, FNetworkGUID> mObjectToNetworkID;
    int32 mFeaturesTicking = 0;

#if DO_ENSURE
public:
    ~FKLDebugImGuiClientManager();

private:
    FObjectKey mEnsureImguiSubsystemKey;
#endif
};

inline void FKLDebugImGuiClientManager::AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage)
{
    mPendingMessages.Emplace(MoveTemp(_PendingMessage));
}

inline bool FKLDebugImGuiClientManager::RequiresGameThreadTick() const
{
    return !mPendingMessages.IsEmpty() || !mPendingFeaturesStatusUpdates.IsEmpty() || !mPendingFeatureRequestUpdate.IsEmpty();
}