// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Factory/KLDebugNetworkingCommandFactory.h"

// modules
#include "User/Networking/Internal/Commands/StaticMemory/KLDebugUserNetworkingCommandManager.h"
#include "User/Networking/Internal/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"
#include "User/Networking/Public/Commands/Interface/KLDebugUserNetworkingCommandInterface.h"

// engine
#include "Math/NumericLimits.h"

void FKLDebugNetworkingCommandFactory::Init()
{
    QUICK_SCOPE_CYCLE_COUNTER(FKLDebugNetworkingCommandFactory_Init);

    FKLDebugUserNetworkingCommandManager& Manager = FKLDebugUserNetworkingCommandManager::Get();

    mIDToCommand.Reserve(static_cast<int32>(Manager.GetEntryCount()));
    mCommandsPool.Init(0, Manager.GetTotalSizeRequired());
    SIZE_T Offset = 0;

    const FKLDebugUserNetworkingCommandManagerEntryBase* CommandEntry = Manager.GetStartEntry();
    while (CommandEntry)
    {
        const IKLDebugUserNetworkingCommandInterface& Interface = CommandEntry->AllocateInPlace(static_cast<void*>(&mCommandsPool[0]));
        const FName& Key = Interface.GetCommandID();
        checkf(Offset < TNumericLimits<uint32>::Max(), TEXT("too many commands"));

#if DO_ENSURE
        if (mIDToCommand.Find(Key) != nullptr)
        {
            ensureAlwaysMsgf(mIDToCommand.Find(Key) == nullptr, TEXT("Multiple commands with the same type. should never happen"));
            continue;
        }
#endif

        mIDToCommand.Emplace(Interface.GetCommandID(), static_cast<uint32>(Offset));
        Offset += CommandEntry->GetSize();
        CommandEntry = CommandEntry->GetNextEntry();
    }
}

const IKLDebugUserNetworkingCommandInterface* FKLDebugNetworkingCommandFactory::TryGetCommand(const FName& _CommandID) const
{
    if (const uint32* Offset = mIDToCommand.Find(_CommandID))
    {
        return reinterpret_cast<const IKLDebugUserNetworkingCommandInterface*>(&mCommandsPool[*Offset]);
    }

    return nullptr;
}