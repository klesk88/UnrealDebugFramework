// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Commands/Interface/KLDebugUserNetworkingCommandInterface.h"
#include "Internal/Commands/Helpers/KLDebugUserNetworkingCommandsRequesterClasses.h"

// engine
#include "Containers/Array.h"
#include "Delegates/DelegateCombinations.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

// c++
#include <type_traits>

class ACharacter;
class UWorld;

DECLARE_DELEGATE_OneParam(FOnKLDebugNewCommand, const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& /*_Input*/);

namespace KL::Debug::Networking::Commands::Internal
{
    KLDEBUGUSERNETWORKING_API void BindOnNewCommand(const FOnKLDebugNewCommand& _Delegate);
    KLDEBUGUSERNETWORKING_API void BroadcastNewRequest(const FKLDebugOnNewCommandInput& _Input);
    KLDEBUGUSERNETWORKING_API UE_NODISCARD bool CanBroadcastRequest();

    template <typename CommandClass>
    void SendCommandInternal(const UWorld& _World, const DebugCommandClientFilter& _ClientFilter)
    {
        QUICK_SCOPE_CYCLE_COUNTER(KL_Debug_Networking_Commands_AddCommand);

        static_assert(!CommandClass::StaticHasContextClass(), "Command passed in requires a context!");

        if (!CanBroadcastRequest())
        {
            ensureMsgf(false, TEXT("We cant send commands. The class responsible to deal with pending requests did not register"));
            return;
        }

        TArray<uint8> DummyContext;
        const FKLDebugOnNewCommandInput Input{ _World, CommandClass::StaticGetCommandID(), _ClientFilter, MoveTemp(DummyContext) };
        BroadcastNewRequest(Input);
    }

    template <typename CommandClass, typename CommandContext>
    void SendCommandWithContextInternal(const UWorld& _World, CommandContext& _CommandContext, const uint32 _OptionalBufferSize, const DebugCommandClientFilter& _ClientFilter)
    {
        QUICK_SCOPE_CYCLE_COUNTER(KL_Debug_Networking_Commands_AddCommandWithContext);

        static_assert(CommandClass::StaticHasContextClass(), "Command passed in do not contain any context! Please add the macro KL_DEBUG_COMMAND_ADD_CONTEXT and pass in as argument the context type you want to use with this command");
        static_assert(sizeof(CommandContext) > 0, "the context class is empty!. It needs to contain some data otherwise do not use the KL_DEBUG_COMMAND_ADD_CONTEXT macro");
        static_assert(std::is_same_v<CommandContext, typename CommandClass::ContextClassType>, "context pass is as argument is not the same as the one pointed by the class");

        if (!CanBroadcastRequest())
        {
            ensureMsgf(false, TEXT("We cant send commands. The class responsible to deal with pending requests did not register"));
            return;
        }

        const int32 ReserveSize = _OptionalBufferSize > static_cast<uint32>(sizeof(CommandContext)) ? static_cast<int32>(_OptionalBufferSize) : static_cast<int32>(sizeof(CommandContext));
        TArray<uint8> ContextData;
        ContextData.Reserve(_OptionalBufferSize);
        FMemoryWriter ContextWriter{ ContextData };
        _CommandContext.Serialize(ContextWriter);

        const FKLDebugOnNewCommandInput Input{ _World, CommandClass::StaticGetCommandID(), _ClientFilter, MoveTemp(ContextData) };
        BroadcastNewRequest(Input);
    }

}    // namespace KL::Debug::Networking::Commands::Internal