#include "Message/Feature/StatusUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"

//module
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

//engine
#include "Serialization/Archive.h"

FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate(const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _ContainerType)
    : mNetworkID(_NetworkID)
    , mContainerType(_ContainerType)
{
}

FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mFullyRemoved;
    Archive << mContainerType;

    if (!mFullyRemoved)
    {
        Archive << mFeatureData;
    }

    ensureMsgf(!mFeatureData.IsEmpty(), TEXT("we should have elements when calling this"));
    ensureMsgf(mContainerType != EImGuiInterfaceType::COUNT, TEXT("invalid container type"));

    switch (mContainerType)
    {
    case EImGuiInterfaceType::SELECTABLE:
        SerializeSelectable(Archive);
        break;
    case EImGuiInterfaceType::UNIQUE:
        SerializeUnique(Archive);
        break;
    default:
        checkNoEntry();
    }
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::SerializeSelectable(FArchive& _Archive)
{
    _Archive << mNetworkID;
}

void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::SerializeUnique(FArchive& _Archive)
{

}
