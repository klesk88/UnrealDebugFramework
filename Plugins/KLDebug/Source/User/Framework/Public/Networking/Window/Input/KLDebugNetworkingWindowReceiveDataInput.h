// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Networking/Message/KLDebugNetworkingMessageTypes.h"
#include "Networking/Window/Input/KLDebugNetworkingWindowInputBase.h"

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugNetworkingWindowReceiveDataInput final : public FKLDebugNetworkingWindowInputBase
{
public:
    explicit FKLDebugNetworkingWindowReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveMessageType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive);

    [[nodiscard]] EKLDebugNetworkReceiveMessageType GetReceiveMessageType() const;

private:
    EKLDebugNetworkReceiveMessageType mReceiveMessageType = EKLDebugNetworkReceiveMessageType::RequestUpdate;
};

inline EKLDebugNetworkReceiveMessageType FKLDebugNetworkingWindowReceiveDataInput::GetReceiveMessageType() const
{
    return mReceiveMessageType;
}