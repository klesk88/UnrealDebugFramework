// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Messages/KLDebugNetworkingArbitrerMessageBase.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGARBITRER_API FKLDebugNetworkingArbitrerMessage_ServerPong final : public FKLDebugNetworkingArbitrerMessageBase<EKLDebugArbitrerMessage::ServerPong>
{
public:
    explicit FKLDebugNetworkingArbitrerMessage_ServerPong() = default;
    explicit FKLDebugNetworkingArbitrerMessage_ServerPong(FArchive& _Archive);

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface
};
