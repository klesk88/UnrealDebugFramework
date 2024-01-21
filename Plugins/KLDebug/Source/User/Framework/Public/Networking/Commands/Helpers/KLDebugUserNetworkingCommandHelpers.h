// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Commands/Helpers/KLDebugUserNetworkingCommandsRequester.h"

// engine
#include "Containers/Array.h"

class UWorld;

/*
 * NOTE:all AddCommands method support adding of commands from multiple threads at the same time
 * Also we need the world as in editor builds server and all clients are spawn usually in the same process. So we need a way to understand from which type of world (server or which client)
 * the request come from so we know which sender pool to send the request to
 */
namespace KL::Debug::Networking::Commands
{
    // add a simple command without any context data. This can be called both from client and server side
    //@param _World the current game world from which we are doing the request
    template <typename CommandClass>
    void SendCommand(const UWorld& _World)
    {
        Internal::SendCommandInternal<CommandClass>(_World, Internal::DebugCommandClientFilter());
    }

    // this allows to pass in the context of the class that will be sync over the network. This can be called both from client and server side
    //@param _World the current game world from which we are doing the request
    //@param _CommandContext the context fully initialized to be serialized over the network
    //@param _OptionalBufferSize optional size that the user can pass in to avoid reallocations of the buffer while serializing the context. If 0 we will get the size of the context class as value
    // NOTE: we expect the context to be fully initialized at this point. Also the class MUST make use
    // of the macro KL_DEBUG_COMMAND_ADD_CONTEXT to allow the system to know what context that class is using
    template <typename CommandClass, typename CommandContext>
    void SendCommandWithContext(const UWorld& _World, CommandContext& _CommandContext, const uint32 _OptionalBufferSize = 0)
    {
        Internal::SendCommandWithContextInternal<CommandClass>(_World, _CommandContext, _OptionalBufferSize, Internal::DebugCommandClientFilter());
    }

    // add a simple command without any context data, and allows to pass a filter that can be used to filter out clients
    // This can be called only from the server side!
    //@param _World the current game world from which we are doing the request
    //@param _OptClientFilter filter that will be used to filter out clients so the message it is sent only to the ones the user want
    template <typename CommandClass>
    void Server_SendCommandToClientsWithFilter(const UWorld& _World, const Internal::DebugCommandClientFilter& _OptClientFilter)
    {
        Internal::SendCommandInternal<CommandClass>(_World, _OptClientFilter);
    }

    // this allows to pass in the context of the class that will be sync over the network and allows to pass a filter that can be used to filter out clients
    // This can be called only from the server side!
    //@param _World the current game world from which we are doing the request
    //@param _CommandContext the context fully initialized to be serialized over the network
    //@param _OptClientFilter filter that will be used to filter out clients so the message it is sent only to the ones the user want
    //@param _OptionalBufferSize optional size that the user can pass in to avoid reallocations of the buffer while serializing the context. If 0 we will get the size of the context class as value
    // NOTE: we expect the context to be fully initialized at this point. Also the class MUST make use
    // of the macro KL_DEBUG_COMMAND_ADD_CONTEXT to allow the system to know what context that class is using
    template <typename CommandClass, typename CommandContext>
    void Server_SendCommandToClientWithContextWithFilter(const UWorld& _World, CommandContext& _CommandContext, const Internal::DebugCommandClientFilter& _OptClientFilter, const uint32 _OptionalBufferSize = 0)
    {
        Internal::SendCommandWithContextInternal<CommandClass>(_World, _CommandContext, _OptionalBufferSize, _OptClientFilter);
    }
}    // namespace KL::Debug::Networking::Commands