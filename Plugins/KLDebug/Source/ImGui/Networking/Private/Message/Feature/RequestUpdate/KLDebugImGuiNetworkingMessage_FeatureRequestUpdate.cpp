// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Feature/RequestUpdate/KLDebugImGuiNetworkingMessage_FeatureRequestUpdate.h"

// module
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate(const FName& _FeatureName, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const EImGuiInterfaceType _ContainerType, const TArray<uint8>& _BufferData)
    : mBufferData(_BufferData)
    , mFeatureNameID(_FeatureName)
    , mFeatureIndex(_FeatureIndex)
    , mContainerType(_ContainerType)
{
}

FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate(const FName& _FeatureName, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _ContainerType, const TArray<uint8>& _BufferData)
    : mBufferData(_BufferData)
    , mFeatureNameID(_FeatureName)
    , mNetworkID(_NetworkID)
    , mFeatureIndex(_FeatureIndex)
    , mContainerType(_ContainerType)
{
}

FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate(FArchive& _Archive)
{
    Serialize(_Archive);
}

void FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mFeatureNameID;
    Archive << mFeatureIndex;
    Archive << mContainerType;
    Archive << mBufferData;

    ensureMsgf(!mBufferData.IsEmpty(), TEXT("we must have data"));

    switch (mContainerType)
    {
    case EImGuiInterfaceType::SELECTABLE:
        Archive << mNetworkID;
        break;
    case EImGuiInterfaceType::UNIQUE:
        break;
    default:
        ensureMsgf(false, TEXT("should never enter here"));
        break;
    }
}
