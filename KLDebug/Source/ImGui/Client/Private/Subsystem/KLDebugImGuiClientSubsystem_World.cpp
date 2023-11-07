//#include "Subsystem/KLDebugImGuiClientSubsystem_World.h"
//
//#include "Client/KLDebugImGuiClientGameThreadContext.h"
//#include "Subsystem/KLDebugImGuiClientSubsystem_Engine.h"
//
////modules
//#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
//#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
//#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"
//#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
//#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
//#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
//#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
//#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"
//#include "Utils/Public/KLDebugLog.h"
//
////engine
//#include "Engine/EngineBaseTypes.h"
//#include "Engine/Engine.h"
//#include "Engine/LocalPlayer.h"
//#include "Engine/World.h"
//#include "GameFramework/PlayerController.h"
//#include "Kismet/KismetSystemLibrary.h"
//
//UKLDebugImGuiClientSubsystem_World* UKLDebugImGuiClientSubsystem_World::TryGetMutable(const UObject& _Object)
//{
//    const UWorld* World = GEngine->GetWorldFromContextObject(&_Object, EGetWorldErrorMode::ReturnNull);
//    return World ? World->GetSubsystem<UKLDebugImGuiClientSubsystem_World>() : nullptr;
//}
//
//bool UKLDebugImGuiClientSubsystem_World::ShouldCreateSubsystem(UObject* _Outer) const
//{
//#if WITH_SERVER_CODE && !WITH_EDITOR
//    return false;
//#else
//    const UWorld* World = Cast<const UWorld>(_Outer);
//    checkf(World != nullptr, TEXT("world must be valid"));
//    return IsValid(*World);
//#endif
//}
//
//bool UKLDebugImGuiClientSubsystem_World::DoesSupportWorldType(const EWorldType::Type _WorldType) const
//{
//    switch (_WorldType)
//    {
//
//    case EWorldType::Type::Game:
//    case EWorldType::Type::Editor:
//    case EWorldType::Type::PIE:
//    case EWorldType::Type::GameRPC:
//        return true;
//    case EWorldType::Type::GamePreview:
//    case EWorldType::Type::EditorPreview:
//    case EWorldType::Type::None:
//    case EWorldType::Type::Inactive:
//        return false;
//    default:
//        ensureMsgf(false, TEXT("case not handled"));
//        return false;
//    }
//}
//
//void UKLDebugImGuiClientSubsystem_World::Deinitialize()
//{
//    Super::Deinitialize();
//
//    ClearWorldDelegates(*GetWorld());
//
//    //if (UKLDebugImGuiClientSubsystem_Engine* EngineSubsystem = UKLDebugImGuiClientSubsystem_Engine::TryGetMutable())
//    //{
//    //    EngineSubsystem->OnWorldChange(nullptr);
//    //}
//}
//
//void UKLDebugImGuiClientSubsystem_World::OnWorldBeginPlay(UWorld& _World)
//{
//    //we can reach here because the world was not yet initialized in editor. so double check at this point that we are
//    //really on a client
//    if (!IsValid(_World))
//    {
//        return;
//    }
//
//    if (UKLDebugImGuiClientSubsystem_Engine* EngineSubsystem = UKLDebugImGuiClientSubsystem_Engine::TryGetMutable())
//    {
//        //EngineSubsystem->OnWorldChange(&_World);
//        InitWorldDelegates(_World);
//    }
//}
//
//bool UKLDebugImGuiClientSubsystem_World::IsValid(const UWorld& _World) const
//{
//    const bool IsClient = !UKismetSystemLibrary::IsServer(&_World);
//    return IsClient;
//}
//
//void UKLDebugImGuiClientSubsystem_World::InitWorldDelegates(UWorld& _World)
//{
//    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = _World.GetSubsystem<UKLDebugImGuiWorldSubsystem>();
//    if (!ImGuiWorldSubsystem)
//    {
//        UE_LOG(LogKL_Debug, Error, TEXT("UKLDebugImGuiClientSubsystem_World::InitWorldDelegates>> UKLDebugImGuiWorldSubsystem not found"));
//        return;
//    }
//
//    FOnImGuiFeatureStateUpdated::FDelegate FeatureUpdateDelagate = FOnImGuiFeatureStateUpdated::FDelegate::CreateUObject(this, &UKLDebugImGuiClientSubsystem_World::OnFeatureUpdate);
//    mOnFeaturesUpdatedDelegateHandle = ImGuiWorldSubsystem->BindOnImGuiFeatureStateUpdated(FeatureUpdateDelagate);
//}
//
//void UKLDebugImGuiClientSubsystem_World::ClearWorldDelegates(UWorld& _World)
//{
//    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = _World.GetSubsystem<UKLDebugImGuiWorldSubsystem>();
//    if (ImGuiWorldSubsystem && mOnFeaturesUpdatedDelegateHandle.IsValid())
//    {
//        ImGuiWorldSubsystem->UnbindOnImGuiFeatureStateUpdated(mOnFeaturesUpdatedDelegateHandle);
//    }
//}
//
//void UKLDebugImGuiClientSubsystem_World::OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData)
//{
//    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientManager_OnFeatureUpdate);
//
//    const AActor* ObjectAsActor = Cast<const AActor>(_FeatureUpdateData.TryGetObject());
//    if (ObjectAsActor && ObjectAsActor->GetLocalRole() == ROLE_Authority)
//    {
//        UE_LOG(LogKL_Debug, Display, TEXT("UKLDebugImGuiClientSubsystem_World::OnFeatureUpdate>> actor [%s] is locally controlled no message sent to server"),
//            *ObjectAsActor->GetName());
//
//        return;
//    }
//
//    TArray<TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, FName>> FeaturesIndexes;
//    FeaturesIndexes.Reserve(_FeatureUpdateData.GetFeatureIterator().GetFeaturesCount());
//
//    FKLDebugImGuiSubsetFeaturesConstIterator& FeaturesIterator = _FeatureUpdateData.GetFeatureIterator();
//    for (; FeaturesIterator; ++FeaturesIterator)
//    {
//        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeaturesIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
//        const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
//        if (!NetworkInterface || !NetworkInterface->Client_InformServerWhenActive())
//        {
//            continue;
//        }
//
//        FeaturesIndexes.Emplace(FeaturesIterator.GetFeatureDataIndex(), FeaturesIterator.GetFeatureNameID());
//        if (_FeatureUpdateData.IsFullyRemoved())
//        {
//            break;
//        }
//    }
//
//    if (FeaturesIndexes.IsEmpty())
//    {
//        return;
//    }
//
//    FNetworkGUID NetworkID;
//    if (const FNetworkGUID* NetworkIDMap = mObjectToNetworkID.Find(_FeatureUpdateData.GetObjectKey()))
//    {
//        NetworkID = *NetworkIDMap;
//    }
//    else
//    {
//        if (_FeatureUpdateData.IsFullyRemoved())
//        {
//            //the feature has been removed but we yet didnt send any message to the server. This can happen if
//            //we select an actor and just press on the remove button, before opening any window which is networked
//            return;
//        }
//
//        if (!_FeatureUpdateData.TryGetObject())
//        {
//            ensureMsgf(false, TEXT("no valid object passed should not be possible"));
//            return;
//        }
//
//        NetworkID = KL::Debug::Networking::Helpers::TryGetNetworkGuid(*_FeatureUpdateData.TryGetObject());
//        if (!NetworkID.IsValid())
//        {
//            ensureMsgf(false, TEXT("no valid network ID"));
//            return;
//        }
//
//        mObjectToNetworkID.Emplace(_FeatureUpdateData.GetObjectKey(), NetworkID);
//    }
//
//    //i dont expect mPendingFeaturesStatusUpdates to have elements but just in case 
//    FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate* FeatureUpdate = nullptr;
//    for (FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& Update : mPendingFeaturesStatusUpdates)
//    {
//        if (Update.Client_IsEqual(_FeatureUpdateData.GetContainerType(), NetworkID))
//        {
//            FeatureUpdate = &Update;
//            break;
//        }
//    }
//
//    if (!FeatureUpdate)
//    {
//        FeatureUpdate = &mPendingFeaturesStatusUpdates.Emplace_GetRef(NetworkID, _FeatureUpdateData.GetContainerType());
//    }
//
//    if (_FeatureUpdateData.IsFullyRemoved())
//    {
//        FeatureUpdate->Client_SetFullyRemoved();
//        mObjectToNetworkID.Remove(_FeatureUpdateData.GetObjectKey());
//    }
//    else
//    {
//        //clear the flag just in case we reenable before send this packet
//        FeatureUpdate->Client_ClearFullyRemoved();
//
//        for (const TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, FName>& FeatureIndexPair : FeaturesIndexes)
//        {
//            FeatureUpdate->Client_AddFeatureUpdate(FeatureIndexPair.Key, FeatureIndexPair.Value, _FeatureUpdateData.IsFeatureAdded());
//        }
//    }
//
//    mShouldTick = true;
//}
//
//void UKLDebugImGuiClientSubsystem_World::Tick(float DeltaTime)
//{
//    const UWorld* World = GetWorld();
//    const ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
//    const APlayerController* PlayerController = LocalPlayer ? LocalPlayer->GetPlayerController(World) : nullptr;
//    if (!PlayerController)
//    {
//        ensureMsgf(false, TEXT("no valid player controller"));
//        mShouldTick = false;
//        return;
//    }
//
//    const FNetworkGUID WorldNetworkID = KL::Debug::Networking::Helpers::TryGetNetworkGuid(*World);
//    const FNetworkGUID ControllerNetworkID = KL::Debug::Networking::Helpers::TryGetNetworkGuid(*PlayerController);
//
//
//    return;
//    //FKLDebugImGuiClientGameThreadContext Context{ *World, mPendingFeaturesStatusUpdates };
//
//    //UKLDebugImGuiClientSubsystem_Engine* EngineSubsystem = UKLDebugImGuiClientSubsystem_Engine::TryGetMutable();
//    //EngineSubsystem->TickOnGameThread(Context);
//}
