// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientManager.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"

// modules
#include "ImGui/Framework/Public/Feature/Input/KLDebugImGuiGatherFeatureInput.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "ImGui/Networking/Public/Message/Feature/DataUpdate/KLDebugImGuiNetworkingMessage_FeatureDataUpdate.h"
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingReceiveDataInput.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingSplittedMessage.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Stats/Stats2.h"
#include "UObject/CoreNet.h"
#include "UObject/NameTypes.h"

#if !NO_LOGGING
// engine
#include "Containers/UnrealString.h"
#endif

void FKLDebugImGuiClientManager::Init()
{
    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    mTempWriteDataBuffer.Reserve(Settings.Client_GetConnectionTempDataSize());
    mPendingFeaturesStatusUpdates.Reserve(30);
}

void FKLDebugImGuiClientManager::GameThread_TickReadData(FKLDebugImGuiClientData& _ClientData, TArray<FKLDebugNetworkingPendingMessage>& _NewData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_TickReadData);

    GameThread_CopyPendingMessages(_ClientData);
    _ClientData.GetFeaturesStatusUpdateMutable().Reset();
    GameThread_ReadMessages(_ClientData.GetWorld(), _NewData);
}

void FKLDebugImGuiClientManager::GameThread_CopyPendingMessages(FKLDebugImGuiClientData& _ClientData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_CopyPendingMessages);

    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate>& NewData = _ClientData.GetFeaturesStatusUpdateMutable();
    if (NewData.IsEmpty())
    {
        return;
    }

    if (mPendingFeaturesStatusUpdates.IsEmpty())
    {
        mPendingFeaturesStatusUpdates = NewData;
        return;
    }

    const int32 Offset = mPendingFeaturesStatusUpdates.Max() - mPendingFeaturesStatusUpdates.Num();
    if (Offset < NewData.Num())
    {
        mPendingFeaturesStatusUpdates.Reserve(mPendingFeaturesStatusUpdates.Max() + (NewData.Num() - Offset));
    }

    for (const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& NewDataUpdate : NewData)
    {
        mPendingFeaturesStatusUpdates.Emplace(NewDataUpdate);
    }
}

void FKLDebugImGuiClientManager::GameThread_ReadMessages(const UWorld& _World, TArray<FKLDebugNetworkingPendingMessage>& _NewData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_ReadMessages);

    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = _World.GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    const UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    if (!ImGuiWorldSubsystem || !ImGuiEngineSubsystem)
    {
        _NewData.Reset();
        ensureMsgf(false, TEXT("not expected"));
        return;
    }

    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

    for (const FKLDebugNetworkingPendingMessage& PendingMessage : _NewData)
    {
        if (PendingMessage.GetMessageEnumType() != static_cast<uint16>(EKLDebugImGuiNetworkMessageTypes::ImGuiMessage))
        {
            UE_LOG(LogKL_Debug, Error, TEXT("received message of wrong type [%d]"), static_cast<int32>(PendingMessage.GetMessageEnumType()));
            continue;
        }

        const EKLDebugImGuiNetworkMessage MessageType = static_cast<EKLDebugImGuiNetworkMessage>(PendingMessage.GetMessageType());
        FMemoryReader MessageDataReader{ PendingMessage.GetMessageData() };
        switch (MessageType)
        {
        case EKLDebugImGuiNetworkMessage::Server_FeatureDataUpdate:
        {
            const FKLDebugImGuiNetworkingMessage_FeatureDataUpdate FeatureDataUpdate{ MessageDataReader };
            if (!FeatureDataUpdate.IsValid())
            {
                break;
            }

            const EImGuiInterfaceType ImGuiInterfaceType = FeatureDataUpdate.Client_GetImGuiInterfaceType();
            const UObject* OwnerObject = FeatureDataUpdate.Client_GetObject(_World);
            if (!OwnerObject)
            {
                break;
            }

            FKLDebugImGuiGatherFeatureInput Input{ FeatureDataUpdate.Client_GetFeatureIndex(), FeatureDataUpdate.Client_GetImGuiInterfaceType(), OwnerObject, FeatureContainerManager };
            ImGuiWorldSubsystem->TryGatherFeatureAndContext(Input);
            IKLDebugImGuiFeatureInterfaceBase* FeatureInterface = Input.TryGetFeatureInterface();
            IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface ? FeatureInterface->TryGetNetworkInterfaceMutable() : nullptr;
            if (!NetworkInterface)
            {
                ensureMsgf(false, TEXT("we should have a valid network interface"));
                break;
            }

            const TArray<uint8>& FeatureDataRcv = FeatureDataUpdate.Client_GetDataArray();
            FMemoryReader MemoryReader{ FeatureDataRcv };
            const FKLDebugImGuiFeature_NetworkingReceiveDataInput RcvDataInput{ _World, Input.TryGetFeatureContext(), MemoryReader };
            NetworkInterface->ReceiveData(RcvDataInput);
            break;
        }
        default:
            ensureMsgf(false, TEXT("message not handle"));
            break;
        }
    }

    _NewData.Reset();
}

void FKLDebugImGuiClientManager::Parallel_TickWriteData(FArchive& _Writer)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_Parallel_TickWriteData);

    const int32 TempWriteBufferInitialSize = mTempWriteDataBuffer.Max();
    Parallel_WritePendingFeaturesStatusUpdate(mTempWriteDataBuffer, _Writer);
    mTempWriteDataBuffer.SetNum(TempWriteBufferInitialSize);
}

void FKLDebugImGuiClientManager::Parallel_WritePendingFeaturesStatusUpdate(TArray<uint8>& _TempData, FArchive& _Archive)
{
    if (mPendingFeaturesStatusUpdates.IsEmpty())
    {
        return;
    }

    FMemoryWriter TempMemoryWriter{ _TempData };
    for (FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& UpdateStatus : mPendingFeaturesStatusUpdates)
    {
        UpdateStatus.Serialize(TempMemoryWriter);
        if (_TempData.IsEmpty())
        {
            continue;
        }

        KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(UpdateStatus, _TempData, _Archive);
        _TempData.Reset();
    }

    mPendingFeaturesStatusUpdates.Reset();
}
