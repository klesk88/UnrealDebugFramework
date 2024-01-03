// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Delegates/DelegateCombinations.h"
#include "HAL/Platform.h"
#include "UObject/NameTypes.h"
#include "UObject/ObjectKey.h"

class FKLDebugOnNewCommandInput;
class IKLDebugUserNetworkingCommandInterface;
class UWorld;

namespace KL::Debug::Networking::Commands::Internal
{
    class FKLDebugOnNewCommandInput;
}

DECLARE_DELEGATE_OneParam(FOnKLDebugNewCommandWorld, const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& /*_Input*/);

namespace KL::Debug::Networking::Commands::Manager
{
    KLDEBUGIMGUINETWORKING_API void Init();
    KLDEBUGIMGUINETWORKING_API void BindOnNewCommandWorldRequest(const FObjectKey& _WorldKey, const FOnKLDebugNewCommandWorld& _Delegate);
    KLDEBUGIMGUINETWORKING_API void UnbindOnNewCommandWorldRequest(const FObjectKey& _WorldKey);
    KLDEBUGIMGUINETWORKING_API UE_NODISCARD const IKLDebugUserNetworkingCommandInterface* TryGetCommand(const FName& _CommandID);
}    // namespace KL::Debug::Networking::Commands::Manager
