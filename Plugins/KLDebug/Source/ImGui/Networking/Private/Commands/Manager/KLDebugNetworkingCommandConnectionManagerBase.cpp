// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Manager/KLDebugNetworkingCommandConnectionManagerBase.h"

// modules
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingMessage.h"
#include "User/Framework/Internal/Networking/Commands/Helpers/KLDebugUserNetworkingCommandsRequester.h"
#include "User/Framework/Public/Networking/Commands/Interface/KLDebugUserNetworkingCommandInterface.h"

// engine
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Stats/Stats.h"
#include "Stats/Stats2.h"

FKLDebugNetworkingCommandConnectionManagerBase::FKLDebugNetworkingCommandConnectionManagerBase()
{
    mTempCommandBuffer.Reserve(500);
    mCompressedTempCommandBuffer.Reserve(500);
}

void FKLDebugNetworkingCommandConnectionManagerBase::AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage)
{
    FMemoryReader Reader{ _PendingMessage.GetMessageData() };
    FKLDebugNetworkingMessage_Command NewCommand{ Reader };
    mPendingCommandsReceived.Emplace(MoveTempIfPossible(NewCommand));
}

void FKLDebugNetworkingCommandConnectionManagerBase::Parallel_Tick(FArchive& _Archive)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingCommandConnectionManagerBase_ParallelTick);

    FKLDebugNetworkingMessage_Command PendingCommand;
    while (mPendingCommandsToSend.Dequeue(PendingCommand))
    {
        mTempCommandBuffer.Reset();
        FMemoryWriter Writer{ mTempCommandBuffer };
        PendingCommand.Serialize(Writer);

        KL::Debug::Networking::Message::PrepareMessageToSend_SplitIfRequired(PendingCommand, mTempCommandBuffer, mCompressedTempCommandBuffer, _Archive);
    }
}
