// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugNetworkingMessage_Base.h"
#include "Message/KLDebugNetworkingMessageDefinitions.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "UObject/NameTypes.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingMessage_Command final : public TKLDebugNetworkingMessage_Base<EKLDebugNetworkingCommandMessage, EKLDebugNetworkingCommandMessage::SyncCommand, static_cast<uint16>(EKLDebugNetworkingMessagesTypes::Command)>
{
public:
    // for queue
    explicit FKLDebugNetworkingMessage_Command() = default;
    explicit FKLDebugNetworkingMessage_Command(const FName& _ID);
    explicit FKLDebugNetworkingMessage_Command(const FName& _ID, TArray<uint8>&& _ContextData);
    explicit FKLDebugNetworkingMessage_Command(FArchive& _Archive);

    UE_NODISCARD const FName& GetCommandID() const;

    UE_NODISCARD const TArray<uint8>& GetCommandContextData() const;
    UE_NODISCARD TArray<uint8>& GetCommandContextDataMutable();

private:
    // IKLDebugNetworkingMessageInterface
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // IKLDebugNetworkingMessageInterface

private:
    FName mCommandID;
    TArray<uint8> mCommandContextData;
};

inline const FName& FKLDebugNetworkingMessage_Command::GetCommandID() const
{
    return mCommandID;
}

inline const TArray<uint8>& FKLDebugNetworkingMessage_Command::GetCommandContextData() const
{
    return mCommandContextData;
}

inline TArray<uint8>& FKLDebugNetworkingMessage_Command::GetCommandContextDataMutable()
{
    return mCommandContextData;
}