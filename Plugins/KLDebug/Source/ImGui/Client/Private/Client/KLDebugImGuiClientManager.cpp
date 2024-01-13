// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientManager.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"

// modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesTickInput.h"
#include "ImGui/Framework/Public/Feature/Input/KLDebugImGuiGatherFeatureInput.h"
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"
#include "ImGui/Framework/Public/Feature/Visualizer/Iterator/KLDebugImGuiFeatureVisualizerIterator.h"
#include "ImGui/Framework/Public/Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "ImGui/Networking/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"
#include "ImGui/Networking/Public/Message/Feature/DataUpdate/KLDebugImGuiNetworkingMessage_FeatureDataUpdate.h"
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingReceiveDataInput.h"
#include "ImGui/User/Public/Feature/Networking/Input/KLDebugImGuiFeature_NetworkingTickInput.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingSplittedMessage.h"

// engine
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/NetworkGuid.h"
#include "Misc/ScopeExit.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Stats/Stats2.h"
#include "UObject/CoreNet.h"
#include "UObject/NameTypes.h"

#if !NO_LOGGING
// engine
#include "Containers/UnrealString.h"
#endif

namespace KL::Debug::Client
{
    template <typename PendingMessageType>
    void CopyPendingMessage(TArray<PendingMessageType>& _GameThreadPendingFeatures, TArray<PendingMessageType>& _ParallelPendingFeatures)
    {
        if (_GameThreadPendingFeatures.IsEmpty())
        {
            return;
        }

        ON_SCOPE_EXIT
        {
            _GameThreadPendingFeatures.Reset();
        };

        if (_ParallelPendingFeatures.IsEmpty())
        {
            _ParallelPendingFeatures = _GameThreadPendingFeatures;
            return;
        }

        const int32 Offset = _ParallelPendingFeatures.Max() - _ParallelPendingFeatures.Num();
        if (Offset < _GameThreadPendingFeatures.Num())
        {
            _ParallelPendingFeatures.Reserve(_ParallelPendingFeatures.Max() + (_GameThreadPendingFeatures.Num() - Offset));
        }

        for (const PendingMessageType& NewDataUpdate : _GameThreadPendingFeatures)
        {
            _ParallelPendingFeatures.Emplace(NewDataUpdate);
        }
    }

    template <typename PendingMessageType>
    void ParallelSendMessage(TArray<uint8>& _TempData, FArchive& _Archive, TArray<PendingMessageType>& _ParallelPendingFeatures)
    {
        if (_ParallelPendingFeatures.IsEmpty())
        {
            return;
        }

        FMemoryWriter TempMemoryWriter{ _TempData };
        for (PendingMessageType& PendingMessage : _ParallelPendingFeatures)
        {
            PendingMessage.Serialize(TempMemoryWriter);
            if (_TempData.IsEmpty())
            {
                continue;
            }

            KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(PendingMessage, _TempData, _Archive);
            _TempData.Reset();
        }

        _ParallelPendingFeatures.Reset();
    }
}    // namespace KL::Debug::Client

/////////////////////////////////////////////////////////////////////////////////////////

FKLDebugImGuiClientManager::FKLDebugImGuiClientManager(const UWorld& _World)
{
    if (UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = UKLDebugImGuiWorldSubsystem::TryGetMutable(_World))
    {
        // we will be bound for the lifetime of the subsystem

        FOnImGuiFeatureStateUpdated::FDelegate FeatureUpdateDelagate = FOnImGuiFeatureStateUpdated::FDelegate::CreateRaw(this, &FKLDebugImGuiClientManager::OnFeatureUpdate);
        FOnFeaturesTick::FDelegate FeatureRequestUpdate = FOnFeaturesTick::FDelegate::CreateRaw(this, &FKLDebugImGuiClientManager::OnFeaturesTick);

        // we can bypass the NODISCARD because this class will leave until the world subsystem is alive
        static_cast<void>(ImGuiWorldSubsystem->BindOnImGuiFeatureStateUpdated(FeatureUpdateDelagate));
        static_cast<void>(ImGuiWorldSubsystem->BindOnFeatureRequestUpdate(FeatureRequestUpdate));
#if DO_ENSURE
        mEnsureImguiSubsystemKey = ImGuiWorldSubsystem;
#endif
    }
    else
    {
        ensureMsgf(false, TEXT("we expect to have a valid imgui world subsystem"));
    }
}

void FKLDebugImGuiClientManager::Init()
{
    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();
    mTempWriteDataBuffer.Reserve(Settings.Client_GetConnectionTempDataSize());
    mTempTickBuffer.Reserve(300);
    mPendingFeaturesStatusUpdates.Reserve(30);
    mPendingMessages.Reserve(10);
    mObjectToNetworkID.Reserve(20);
}

void FKLDebugImGuiClientManager::GameThread_Tick(const UWorld& _World)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_TickReadData);

    // we need to perform this cpy. The game thread buffers are filled from the callback of the imgui subsystem
    // the update buffer instead is accessed from our thread. To avoid locks we cpy the data here.
    // at this point we are sure that the client thread is not ticking (we are locking it) and the imgui subsystem is not tickign as well
    //(we are on a different part of the frame)
    KL::Debug::Client::CopyPendingMessage(mPendingFeaturesStatusUpdates, mParallelPendingFeaturesStatusUpdates);
    KL::Debug::Client::CopyPendingMessage(mPendingFeatureRequestUpdate, mParallelPendingFeatureRequestUpdate);
    GameThread_ReadMessages(_World);
}

void FKLDebugImGuiClientManager::GameThread_ReadMessages(const UWorld& _World)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_ReadMessages);

    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = _World.GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    const UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    if (!ImGuiWorldSubsystem || !ImGuiEngineSubsystem)
    {
        mPendingMessages.Reset();
        ensureMsgf(false, TEXT("not expected"));
        return;
    }

    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

    for (const FKLDebugNetworkingPendingMessage& PendingMessage : mPendingMessages)
    {
        if (PendingMessage.GetMessageEnumType() != static_cast<uint16>(EKLDebugImGuiNetworkMessageTypes::ImGuiMessage))
        {
            UE_LOG(LogKLDebug_Networking, Error, TEXT("received message of wrong type [%d]"), static_cast<int32>(PendingMessage.GetMessageEnumType()));
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
            NetworkInterface->Client_ReceiveData(RcvDataInput);
            break;
        }
        default:
            ensureMsgf(false, TEXT("message not handle"));
            break;
        }
    }

    mPendingMessages.Reset();
}

void FKLDebugImGuiClientManager::Parallel_TickWriteData(FArchive& _Writer)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_Parallel_TickWriteData);

    const int32 TempWriteBufferInitialSize = mTempWriteDataBuffer.Max();
    KL::Debug::Client::ParallelSendMessage(mTempWriteDataBuffer, _Writer, mParallelPendingFeaturesStatusUpdates);
    KL::Debug::Client::ParallelSendMessage(mTempWriteDataBuffer, _Writer, mParallelPendingFeatureRequestUpdate);
    mTempWriteDataBuffer.SetNum(TempWriteBufferInitialSize);
}

void FKLDebugImGuiClientManager::OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_OnFeatureUpdate);

    const AActor* ObjectAsActor = Cast<const AActor>(_FeatureUpdateData.TryGetObject());
    if (ObjectAsActor && ObjectAsActor->GetLocalRole() == ROLE_Authority)
    {
        UE_LOG(LogKLDebug_Networking, Display, TEXT("FKLDebugImGuiClientManager::OnFeatureUpdate>> actor [%s] is locally controlled no message sent to server"), *ObjectAsActor->GetName());

        return;
    }

    TArray<TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, FName>> FeaturesIndexes;

    if (_FeatureUpdateData.IsFullyRemoved())
    {
        FKLDebugImGuiSubsetFeaturesConstIterator& FeaturesIterator = _FeatureUpdateData.GetFeatureIterator();
        for (; FeaturesIterator; ++FeaturesIterator)
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeaturesIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
            const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
            if (NetworkInterface && NetworkInterface->RequireClientTick())
            {
                --mFeaturesTicking;
            }
        }
    }
    else
    {
        FeaturesIndexes.Reserve(_FeatureUpdateData.GetFeatureIterator().GetFeaturesCount());

        FKLDebugImGuiSubsetFeaturesConstIterator& FeaturesIterator = _FeatureUpdateData.GetFeatureIterator();
        for (; FeaturesIterator; ++FeaturesIterator)
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeaturesIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
            const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
            if (!NetworkInterface)
            {
                continue;
            }

            if (NetworkInterface->RequireClientTick())
            {
                ++mFeaturesTicking;
            }

            if (!NetworkInterface->RequireServerTick())
            {
                continue;
            }

            FeaturesIndexes.Emplace(FeaturesIterator.GetFeatureDataIndex(), FeaturesIterator.GetFeatureNameID());
        }

        if (FeaturesIndexes.IsEmpty())
        {
            return;
        }
    }

    FNetworkGUID NetworkID;
    if (const FNetworkGUID* NetworkIDMap = mObjectToNetworkID.Find(_FeatureUpdateData.GetObjectKey()))
    {
        NetworkID = *NetworkIDMap;
    }
    else
    {
        if (_FeatureUpdateData.IsFullyRemoved())
        {
            // the feature has been removed but we yet didnt send any message to the server. This can happen if
            // we select an actor and just press on the remove button, before opening any window which is networked
            return;
        }

        if (!_FeatureUpdateData.TryGetObject())
        {
            ensureMsgf(false, TEXT("no valid object passed should not be possible"));
            return;
        }

        checkf(_FeatureUpdateData.GetObjectKey().ResolveObjectPtr() != nullptr, TEXT("must be valid"));
        NetworkID = KL::Debug::ImGuiNetworking::Helpers::TryGetNetworkGuid(*_FeatureUpdateData.GetObjectKey().ResolveObjectPtr());
        if (!NetworkID.IsValid())
        {
            ensureMsgf(false, TEXT("no valid network ID"));
            return;
        }

        mObjectToNetworkID.Emplace(_FeatureUpdateData.GetObjectKey(), NetworkID);
    }

    // i dont expect mPendingFeaturesStatusUpdates to have elements but just in case
    FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate* FeatureUpdate = nullptr;
    for (FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& Update : mPendingFeaturesStatusUpdates)
    {
        if (Update.Client_IsEqual(_FeatureUpdateData.GetContainerType(), NetworkID))
        {
            FeatureUpdate = &Update;
            break;
        }
    }

    if (!FeatureUpdate)
    {
        FeatureUpdate = &mPendingFeaturesStatusUpdates.Emplace_GetRef(NetworkID, _FeatureUpdateData.GetContainerType());
    }

    if (_FeatureUpdateData.IsFullyRemoved())
    {
        FeatureUpdate->Client_SetFullyRemoved();
        mObjectToNetworkID.Remove(_FeatureUpdateData.GetObjectKey());
    }
    else
    {
        // clear the flag just in case we reenable before send this packet
        FeatureUpdate->Client_ClearFullyRemoved();

        for (const TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, FName>& FeatureIndexPair : FeaturesIndexes)
        {
            FeatureUpdate->Client_AddFeatureUpdate(FeatureIndexPair.Key, FeatureIndexPair.Value, _FeatureUpdateData.IsFeatureAdded());
        }
    }
}

void FKLDebugImGuiClientManager::OnFeaturesTick(const FKLDebugImGuiFeaturesTickInput& _Input)
{
    if (mFeaturesTicking == 0)
    {
        return;
    }

    TickUniqueFeatures(_Input);
    TickObjectFeatures(_Input);

    mTempTickBuffer.Reset();
}

void FKLDebugImGuiClientManager::TickUniqueFeatures(const FKLDebugImGuiFeaturesTickInput& _Input)
{
    KL::Debug::ImGui::Features::Types::FeatureEnableSet& Flags = _Input.GetFeatureFlagsMutable();
    const UWorld& CurrentWorld = _Input.GetOwnerWorld();

    if (_Input.GetUniqueFeaturesMutable().IsValid())
    {
        auto TickUniqueLambda = [&CurrentWorld, &Flags, this](const FKLDebugImGuiFeatureVisualizerEntry& _FeatureData, FKLDebugImGuiFeatureVisualizerIterator& _Iterator) -> void {
            IKLDebugImGuiFeatureInterface_Unique& Feature = _Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Unique>();
            if (IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = Feature.TryGetNetworkInterfaceMutable())
            {
                mTempTickBuffer.Reset();
                FMemoryWriter TempWriter{ mTempTickBuffer };
                FKLDebugImGuiFeature_NetworkingTickInput TickInput{ CurrentWorld, TempWriter };
                NetworkInterface->Client_Tick(TickInput);

                if (TickInput.ShouldUpdateSceneProxy())
                {
                    Flags[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] = 1;
                }

                if (!mTempTickBuffer.IsEmpty())
                {
                    mPendingFeatureRequestUpdate.Emplace(Feature.GetFeatureNameID(), _Iterator.GetFeatureDataIndex(), EImGuiInterfaceType::UNIQUE, mTempTickBuffer);
                }
            }
        };

        _Input.GetUniqueFeaturesMutable().ApplyDelegateMutable(TickUniqueLambda, _Input.GetContainerManagerMutable());
    }
}

void FKLDebugImGuiClientManager::TickObjectFeatures(const FKLDebugImGuiFeaturesTickInput& _Input)
{
    KL::Debug::ImGui::Features::Types::FeatureEnableSet& Flags = _Input.GetFeatureFlagsMutable();
    const UWorld& CurrentWorld = _Input.GetOwnerWorld();

    auto TickObjectLambda = [&CurrentWorld, &Flags, this](const UObject& _Owner, const FKLDebugImGuiFeatureVisualizerEntry& _FeatureData, FKLDebugImGuiFeatureVisualizerIterator& _Iterator) -> void {
        IKLDebugImGuiFeatureInterface_Selectable& Feature = _Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Selectable>();
        if (IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = Feature.TryGetNetworkInterfaceMutable())
        {
            mTempTickBuffer.Reset();
            FMemoryWriter TempWriter{ mTempTickBuffer };
            FKLDebugImGuiFeature_NetworkingTickInput TickInput{ CurrentWorld, TempWriter };
            NetworkInterface->Client_Tick(TickInput);

            if (TickInput.ShouldUpdateSceneProxy())
            {
                Flags[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] = 1;
            }

            if (!mTempTickBuffer.IsEmpty())
            {
                const FNetworkGUID* NetworkID = mObjectToNetworkID.Find(FObjectKey(&_Owner));
                if (NetworkID && NetworkID->IsValid())
                {
                    mPendingFeatureRequestUpdate.Emplace(Feature.GetFeatureNameID(), _Iterator.GetFeatureDataIndex(), *NetworkID, EImGuiInterfaceType::SELECTABLE, mTempTickBuffer);
                }
                else
                {
                    ensureMsgf(false, TEXT("network id should be valid"));
                }
            }
        }
    };

    for (FKLDebugImGuiFeatureVisualizer_Selectable& ObjectFeatures : _Input.GetObjectFeaturesMutable())
    {
        ObjectFeatures.ApplyDelegateMutable(TickObjectLambda, _Input.GetContainerManagerMutable());
    }
}

#if DO_ENSURE

FKLDebugImGuiClientManager::~FKLDebugImGuiClientManager()
{
    ensureMsgf(!mEnsureImguiSubsystemKey.ResolveObjectPtr(), TEXT("imgui subsystem should not be valid anymore"));
}

#endif