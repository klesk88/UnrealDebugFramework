// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystem/Engine/KLDebugImGuiClientData.h"

// modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "ImGui/Networking/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"
#include "ImGui/Networking/Public/Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Engine/EngineBaseTypes.h"
#include "Kismet/KismetSystemLibrary.h"

FKLDebugImGuiClientData::FKLDebugImGuiClientData(const UWorld& _World, const FDelegateHandle& _DelegateHandle)
    : mWorldKey(&_World)
    , mOnFeaturesUpdatedDelegateHandle(_DelegateHandle)
{
}

void FKLDebugImGuiClientData::Shutdown(UKLDebugImGuiWorldSubsystem& _ImGuiWorldSubsystem)
{
    if (mOnFeaturesUpdatedDelegateHandle.IsValid())
    {
        _ImGuiWorldSubsystem.UnbindOnImGuiFeatureStateUpdated(mOnFeaturesUpdatedDelegateHandle);
    }
}

bool FKLDebugImGuiClientData::OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData)
{
    const AActor* ObjectAsActor = Cast<const AActor>(_FeatureUpdateData.TryGetObject());
    if (ObjectAsActor && ObjectAsActor->GetLocalRole() == ROLE_Authority)
    {
        UE_LOG(LogKL_Debug, Display, TEXT("UKLDebugImGuiClientSubsystem_World::OnFeatureUpdate>> actor [%s] is locally controlled no message sent to server"), *ObjectAsActor->GetName());

        return false;
    }

    TArray<TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, FName>> FeaturesIndexes;
    FeaturesIndexes.Reserve(_FeatureUpdateData.GetFeatureIterator().GetFeaturesCount());

    FKLDebugImGuiSubsetFeaturesConstIterator& FeaturesIterator = _FeatureUpdateData.GetFeatureIterator();
    for (; FeaturesIterator; ++FeaturesIterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeaturesIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        const IKLDebugImGuiFeature_NetworkingInterface* NetworkInterface = FeatureInterface.TryGetNetworkInterface();
        if (!NetworkInterface || !NetworkInterface->Client_InformServerWhenActive())
        {
            continue;
        }

        FeaturesIndexes.Emplace(FeaturesIterator.GetFeatureDataIndex(), FeaturesIterator.GetFeatureNameID());
        if (_FeatureUpdateData.IsFullyRemoved())
        {
            break;
        }
    }

    if (FeaturesIndexes.IsEmpty())
    {
        return false;
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
            return false;
        }

        if (!_FeatureUpdateData.TryGetObject())
        {
            ensureMsgf(false, TEXT("no valid object passed should not be possible"));
            return false;
        }

        checkf(_FeatureUpdateData.GetObjectKey().ResolveObjectPtr() != nullptr, TEXT("must be valid"));
        NetworkID = KL::Debug::ImGuiNetworking::Helpers::TryGetNetworkGuid(*_FeatureUpdateData.GetObjectKey().ResolveObjectPtr());
        if (!NetworkID.IsValid())
        {
            ensureMsgf(false, TEXT("no valid network ID"));
            return false;
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

    return true;
}