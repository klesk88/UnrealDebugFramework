// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugNetworkingMessage_Base.h"
#include "Message/KLDebugNetworkingMessageDefinitions.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingMessage_CheckIfAlive final : public TKLDebugNetworkingMessage_Base<EKLDebugNetworkingMessage, EKLDebugNetworkingMessage::IsConnectionAlive, static_cast<uint16>(EKLDebugNetworkingMessagesTypes::NetworkingRuntime)>
{
public:
    FKLDebugNetworkingMessage_CheckIfAlive() = default;
    explicit FKLDebugNetworkingMessage_CheckIfAlive(FArchive& _Archive);

    // IKLDebugNetworkingMessageInterface
    [[nodiscard]] bool SupportsEmptyData() const final;
    // IKLDebugNetworkingMessageInterface

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface
};

inline bool FKLDebugNetworkingMessage_CheckIfAlive::SupportsEmptyData() const
{
    return true;
}