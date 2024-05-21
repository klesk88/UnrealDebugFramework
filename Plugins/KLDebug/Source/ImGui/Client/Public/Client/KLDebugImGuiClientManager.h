// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Networking/Public/Message/Feature/RequestUpdate/KLDebugImGuiNetworkingMessage_FeatureRequestUpdate.h"
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "ImGui/Networking/Public/Message/Window/RequestUpdate/KLDebugNetworkingMessage_WindowRequestUpdate.h"
#include "ImGui/Networking/Public/Message/Window/Toogle/KLDebugNetworkingMessage_WindowToogle.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingMessage.h"
#include "User/Framework/Public/Window/KLDebugWindowTypes.h"

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FArchive;
class FKLDebugFrameworkBottomBarManager;
class FKLDebugFrameworkModeManager;
class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTickInput;
class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiNetworkingMessage_FeatureDataUpdate;
class FKLDebugNetworkingMessage_WindowDataUpdate;
class FKLDebugNetworkingPendingMessage;
class IKLDebugNetworkingGetterInterface;
class UKLDebugImGuiWorldSubsystem;
class UWorld;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientManager final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiClientManager(const UWorld& _World);

    void Init();

    void AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage);

    void GameThread_Tick(const UWorld& _World);
    void Parallel_TickWriteData(FArchive& _Writer);

    [[nodiscard]] bool RequiresGameThreadTick() const;

private:
    void GameThread_ReadMessages(const UWorld& _World);

    void GameThread_ReadFeatureUpdate(const UWorld& _World, const FKLDebugImGuiNetworkingMessage_FeatureDataUpdate& _Message, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, const UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem) const;
    void GameThread_ReadWindowUpdate(const UWorld& _World, const UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, FKLDebugNetworkingMessage_WindowDataUpdate& _Message, FKLDebugFrameworkBottomBarManager& _BottomManager, FKLDebugFrameworkModeManager& _ModeManager) const;

    void OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);
    void OnFeaturesTick(const FKLDebugImGuiFeaturesTickInput& _Input);
    void OnWindowStatusChange(const int32 _WindowIndex, const EKLDebugWindowTypes _WindowType, const FName& _WindowID, const IKLDebugNetworkingGetterInterface* _PrevNetworkInterfaceChecker, const IKLDebugNetworkingGetterInterface* _NetworkInterfaceChecker);
    void OnShouldTick(bool& _ShouldTick) const;

    void TickUniqueFeatures(const FKLDebugImGuiFeaturesTickInput& _Input);
    void TickObjectFeatures(const FKLDebugImGuiFeaturesTickInput& _Input);
    void TickWindow(const FKLDebugImGuiFeaturesTickInput& _Input);

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mParallelPendingFeaturesStatusUpdates;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TArray<FKLDebugNetworkingPendingMessage> mPendingMessages;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate> mParallelPendingFeatureRequestUpdate;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate> mPendingFeatureRequestUpdate;
    TArray<FKLDebugNetworkingMessage_WindowToogle> mParallelWindowToogle;
    TArray<FKLDebugNetworkingMessage_WindowToogle> mPendingWindowToogle;
    TArray<FKLDebugNetworkingMessage_WindowRequestUpdate> mParallelWindowRequestUpdate;
    TArray<FKLDebugNetworkingMessage_WindowRequestUpdate> mPendingWindowRequestUpdate;
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
    return !mPendingMessages.IsEmpty() || !mPendingFeaturesStatusUpdates.IsEmpty() || !mPendingFeatureRequestUpdate.IsEmpty() || !mPendingWindowToogle.IsEmpty();
}

inline void FKLDebugImGuiClientManager::OnShouldTick(bool& _ShouldTick) const
{
    _ShouldTick |= mFeaturesTicking > 0;
}
