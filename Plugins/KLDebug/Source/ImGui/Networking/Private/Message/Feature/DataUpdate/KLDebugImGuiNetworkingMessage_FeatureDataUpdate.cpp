// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Feature/DataUpdate/KLDebugImGuiNetworkingMessage_FeatureDataUpdate.h"

// module
#include "ImGui/Networking/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::FKLDebugImGuiNetworkingMessage_FeatureDataUpdate(const EKLDebugNetworkReceiveMessageType _ReceiveType, const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _InterfaceType, const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
    : mOwnerObjectNetworkID(_NetworkID)
    , mFeatureNameID(_FeatureNameID)
    , mFeatureIndex(_FeatureIndex)
    , mInterfaceType(_InterfaceType)
    , mReceiveType(_ReceiveType)
{
}

void FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();
    Archive << mInterfaceType;
    Archive << mReceiveType;

    switch (mInterfaceType)
    {
    case EImGuiInterfaceType::SELECTABLE:
        Archive << mOwnerObjectNetworkID;
        break;
    case EImGuiInterfaceType::UNIQUE:
        break;
    }

    Archive << mFeatureNameID;
    Archive << mFeatureIndex;

    if (Archive.IsLoading())
    {
        const int32 Size = static_cast<int32>(Archive.TotalSize() - Archive.Tell());
        if (Size > 0)
        {
            mFeatureData.AddUninitialized(Size);
            Archive.Serialize(mFeatureData.GetData(), Size);
        }
        else
        {
            ensureMsgf(false, TEXT("size is wrong"));
        }
    }
    else
    {
        ensureMsgf(mFeatureData.IsEmpty(), TEXT("fata is serialized on the server outside of this message we should be empty"));
    }
}

UObject* FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::Client_GetObjectMutable(const UWorld& _World) const
{
    return mOwnerObjectNetworkID.IsValid() ? KL::Debug::ImGuiNetworking::Helpers::TryGetObjectFromNetworkGUIDMutable(_World, mOwnerObjectNetworkID) : nullptr;
}
