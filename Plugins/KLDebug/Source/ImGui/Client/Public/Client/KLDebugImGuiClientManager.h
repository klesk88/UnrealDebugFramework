// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingMessage.h"

// engine
#include "Containers/Array.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FArchive;
class FKLDebugImGuiClientData;
class FKLDebugNetworkingPendingMessage;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientManager final : public FNoncopyable
{
public:
    void Init();

    void AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage);

    void GameThread_TickReadData(FKLDebugImGuiClientData& _ClientData);
    void Parallel_TickWriteData(FArchive& _Writer);

    UE_NODISCARD bool HasPendingData() const;

private:
    void GameThread_CopyPendingMessages(FKLDebugImGuiClientData& _ClientData);
    void GameThread_ReadMessages(const UWorld& _World);

    void Parallel_WritePendingFeaturesStatusUpdate(TArray<uint8>& _TempData, FArchive& _Archive);

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TArray<FKLDebugNetworkingPendingMessage> mPendingMessages;
    TArray<uint8> mTempWriteDataBuffer;
};

inline void FKLDebugImGuiClientManager::AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage)
{
    mPendingMessages.Emplace(MoveTemp(_PendingMessage));
}

inline bool FKLDebugImGuiClientManager::HasPendingData() const
{
    return !mPendingMessages.IsEmpty();
}