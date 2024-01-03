// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
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
class FKLDebugNetworkingPendingMessage;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientManager final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientManager(const UWorld& _World);

    void Init();

    void AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage);

    void GameThread_TickReadData(const UWorld& _World);
    void Parallel_TickWriteData(FArchive& _Writer);

    UE_NODISCARD bool HasPendingData() const;

private:
    void GameThread_ReadMessages(const UWorld& _World);
    void GameThread_CopyPendingMessages();

    void Parallel_WritePendingFeaturesStatusUpdate(TArray<uint8>& _TempData, FArchive& _Archive);

    void OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);
    UE_NODISCARD bool OnFeatureUpdateInternal(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mParallelPendingFeaturesStatusUpdates;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TArray<FKLDebugNetworkingPendingMessage> mPendingMessages;
    TArray<uint8> mTempWriteDataBuffer;
    TMap<FObjectKey, FNetworkGUID> mObjectToNetworkID;

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

inline bool FKLDebugImGuiClientManager::HasPendingData() const
{
    return !mPendingMessages.IsEmpty();
}