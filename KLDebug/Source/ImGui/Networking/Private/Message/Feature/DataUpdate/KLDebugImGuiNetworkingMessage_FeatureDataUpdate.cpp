#include "Message/Feature/DataUpdate/KLDebugImGuiNetworkingMessage_FeatureDataUpdate.h"

//module
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::FKLDebugImGuiNetworkingMessage_FeatureDataUpdate(const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _InterfaceType, const FName& _FeatureNameID, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
    : mOwnerObjectNetworkID(_NetworkID)
    , mFeatureNameID(_FeatureNameID)
    , mFeatureIndex(_FeatureIndex)
    , mInterfaceType(_InterfaceType)
{
}

FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::FKLDebugImGuiNetworkingMessage_FeatureDataUpdate(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();
    Archive << mInterfaceType;
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

const UObject* FKLDebugImGuiNetworkingMessage_FeatureDataUpdate::Client_GetObject(const UWorld& _World) const
{
    return KL::Debug::Networking::Helpers::TryGetObjectFromNetworkGUID(_World, mOwnerObjectNetworkID);
}
