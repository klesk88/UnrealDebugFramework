// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Helpers/KLDebugUserNetworkingCommandsRequesterClasses.h"

namespace KL::Debug::Networking::Commands::Internal
{
    FKLDebugOnNewCommandInput::FKLDebugOnNewCommandInput(const UWorld& _World, const FName& _CommandID, const DebugCommandClientFilter& _ClientFilter, TArray<uint8>&& _ContextData)
        : mWorld(_World)
        , mCommandID(_CommandID)
        , mClientFilter(_ClientFilter)
        , mContextData(MoveTemp(_ContextData))
    {
    }
}    // namespace KL::Debug::Networking::Commands::Internal