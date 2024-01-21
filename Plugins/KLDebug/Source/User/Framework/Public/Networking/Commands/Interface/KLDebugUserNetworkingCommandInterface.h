// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Commands/Interface/KLDebugUserNetworkingCommandInterfaceInternal.h"

// engine
#include "HAL/Platform.h"

/*
 * Base class for any command to extend.
 * IMPORTANT: in the .cpp file please use the macro KL_DEBUG_CREATE_COMMAND() so that the class can auto subscribe to the system.
 * There are also available 2 different macros to use in the definition of the class, depending if it has a context (need to send data over the network) or not
 * KL_DEBUG_COMMAND_GENERATE_WITHOUT_CONTEXT -> allows to pass the class ID (generally the class name itself)
 *                                              In this case you will need to declare the following methods that will be call when we want to execute the command:
 *
 *                                              void Client_Execute(FKLDebugUserNetworkingCommandExecuteInput_Client& _Input) const;
 *                                              void Server_Execute(FKLDebugUserNetworkingCommandExecuteInput_Server& _Input) const;
 *
 * KL_DEBUG_COMMAND_GENERATE_WITH_CONTEXT -> allows to pass the class ID (generally the class name itself) and the context class (MyContextClassType)
 *                                           that is it used to sync across the network data.
 *                                           In this case you will need to declare the following methods that will be call when we want to execute the command:
 *
 *                                           void Client_Execute(FKLDebugUserNetworkingCommandExecuteInput_Client& _Input, MyContextClassType& _Context) const;
 *                                           void Server_Execute(FKLDebugUserNetworkingCommandExecuteInput_Server& _Input, MyContextClassType& _Context) const;
 *
 * For example:
 * class FClassCommandNoContext final : public IKLDebugUserNetworkingCommandInterface
 * {
 *      KL_DEBUG_COMMAND_GENERATE_WITHOUT_CONTEXT(FClassCommandNoContext)
 *
 *      void Client_Execute(FKLDebugUserNetworkingCommandExecuteInput_Client& _Input) const;
 *      void Server_Execute(FKLDebugUserNetworkingCommandExecuteInput_Server& _Input) const;
 *      ...
 * };
 *
 * FClassCommandNoContext.cpp
 *
 * KL_DEBUG_CREATE_COMMAND(FClassCommandNoContext)
 *
 * or, having a context class, :
 *
 * class MyContextClass final : public IKLDebugUserNetworkingCommandContexInterface
 * {
 *  ...
 * };
 *
 * class FClassCommandWithContext final : public IKLDebugUserNetworkingCommandInterface
 * {
 *      KL_DEBUG_COMMAND_GENERATE_WITHOUT_CONTEXT(FClassCommandWithContext, MyContextClass)
 *
 *      void Client_Execute(FKLDebugUserNetworkingCommandExecuteInput_Client& _Input, MyContextClass& _Context) const;
 *      void Server_Execute(FKLDebugUserNetworkingCommandExecuteInput_Server& _Input, MyContextClass& _Context) const;
 *      ...
 * };
 *
 * FClassCommandWithContext.cpp
 *
 * KL_DEBUG_CREATE_COMMAND(FClassCommandWithContext)
 */

class KLDEBUGUSERFRAMEWORK_API IKLDebugUserNetworkingCommandInterface : public IKLDebugUserNetworkingCommandInterfaceInternal
{
public:
    enum class EServerCommandExecutionType : uint8
    {
        Server_Execute = 0,                          // execute command on the server and nothing else
        Server_ExecuteAndBroadcast,                  // send from the client a command to the server which will be just broadcasted to all the other clients (not to the one is coming from)
        Server_ExecuteAndBroadcastSenderIncluded,    // send from the client a command to the server which will be executed and broadcasted to all the other clients (also to the one the message is coming from)
        Server_BroadcastOnly,                        // broadcast the message only to the other clients (not to the one is coming from)
    };

public:
    // allows to determine if the command should run on the server or not. This is useful if we want,
    // for example, to send a command from one client to another and we use the server only as a way to broadcast
    // the message.
    // by default we execute the command on the server
    UE_NODISCARD virtual EServerCommandExecutionType Server_ShouldExecute() const;
};

inline IKLDebugUserNetworkingCommandInterface::EServerCommandExecutionType IKLDebugUserNetworkingCommandInterface::Server_ShouldExecute() const
{
    return EServerCommandExecutionType::Server_Execute;
}
