// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Commands/Message/KLDebugNetworkingMessage_Command.h"

// modules
#include "User/Framework/Internal/Networking/Commands/Helpers/KLDebugUserNetworkingCommandsRequester.h"

// engine
#include "Containers/Array.h"
#include "Containers/Queue.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class FArchive;
class FKLDebugNetworkingPendingMessage;
class FKLDebugUserNetworkingCommandExecuteInput;
class IKLDebugUserNetworkingCommandInterface;
class UWorld;

class KLDEBUGIMGUINETWORKING_API FKLDebugNetworkingCommandConnectionManagerBase : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingCommandConnectionManagerBase();
    virtual ~FKLDebugNetworkingCommandConnectionManagerBase() = default;

    UE_NODISCARD bool HasPendingData() const;
    void AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage);
    virtual void AddSendCommand(FKLDebugNetworkingMessage_Command&& _NewCommand);

    void Parallel_Tick(FArchive& _Archive);

protected:
    TArray<FKLDebugNetworkingMessage_Command> mPendingCommandsReceived;
    TQueue<FKLDebugNetworkingMessage_Command, EQueueMode::Mpsc> mPendingCommandsToSend;

private:
    TArray<uint8> mTempCommandBuffer;
    TArray<uint8> mCompressedTempCommandBuffer;
};

inline bool FKLDebugNetworkingCommandConnectionManagerBase::HasPendingData() const
{
    return !mPendingCommandsReceived.IsEmpty();
}

inline void FKLDebugNetworkingCommandConnectionManagerBase::AddSendCommand(FKLDebugNetworkingMessage_Command&& _NewCommand)
{
    mPendingCommandsToSend.Enqueue(MoveTemp(_NewCommand));
}