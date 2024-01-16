// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Window/Input/KLDebugNetworkingWindowInputBase.h"

// modules
#include "ImGui/Networking/Public/Message/KLDebugImGuiNetworkingMessageTypes.h"

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugNetworkingWindowReceiveDataInput final : public FKLDebugNetworkingWindowInputBase
{
public:
    explicit FKLDebugNetworkingWindowReceiveDataInput(const EKLDebugImGuiNetworkReceiveMessageType _ReceiveMessageType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive);

    UE_NODISCARD EKLDebugImGuiNetworkReceiveMessageType GetReceiveMessageType() const;

private:
    EKLDebugImGuiNetworkReceiveMessageType mReceiveMessageType = EKLDebugImGuiNetworkReceiveMessageType::RequestUpdate;
};

inline EKLDebugImGuiNetworkReceiveMessageType FKLDebugNetworkingWindowReceiveDataInput::GetReceiveMessageType() const
{
    return mReceiveMessageType;
}