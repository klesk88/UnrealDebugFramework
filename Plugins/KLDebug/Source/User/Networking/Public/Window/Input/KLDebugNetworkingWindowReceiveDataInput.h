// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugNetworkingMessageTypes.h"
#include "Window/Input/KLDebugNetworkingWindowInputBase.h"

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugNetworkingWindowReceiveDataInput final : public FKLDebugNetworkingWindowInputBase
{
public:
    explicit FKLDebugNetworkingWindowReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveMessageType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive);

    UE_NODISCARD EKLDebugNetworkReceiveMessageType GetReceiveMessageType() const;

private:
    EKLDebugNetworkReceiveMessageType mReceiveMessageType = EKLDebugNetworkReceiveMessageType::RequestUpdate;
};

inline EKLDebugNetworkReceiveMessageType FKLDebugNetworkingWindowReceiveDataInput::GetReceiveMessageType() const
{
    return mReceiveMessageType;
}