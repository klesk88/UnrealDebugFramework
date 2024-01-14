// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntry.h"

// engine
#include "Serialization/Archive.h"
#include "Templates/IsConstructible.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

// c++
#include <type_traits>

class FKLDebugUserNetworkingCommandExecuteInput_Client;
class FKLDebugUserNetworkingCommandExecuteInput_Server;

GENERATE_STATIC_FUNCTION_CHECK(StaticGetCommandID, const FName&)

namespace KL::Debug::User::Commands::MacroHelpers
{
    /*This will allow to call the correct execute function (with context or without) directly. The user is expected to create the correct Execute signature*/
    template <typename CommandType, typename ContextType, typename InputType>
    void ExecuteCommand(const CommandType& _Command, const InputType& _Input, FArchive& _ArchiveContext)
    {
        constexpr bool IsClient = std::is_same_v<InputType, FKLDebugUserNetworkingCommandExecuteInput_Client>;

        if constexpr (CommandType::StaticHasContextClass())
        {
            ContextType Context{ _ArchiveContext };
            if constexpr (IsClient)
            {
                _Command.Client_Execute(_Input, Context);
            }
            else
            {
                _Command.Server_Execute(_Input, Context);
            }
        }
        else
        {
            if constexpr (IsClient)
            {
                _Command.Client_Execute(_Input);
            }
            else
            {
                _Command.Server_Execute(_Input);
            }
        }
    }
}    // namespace KL::Debug::User::Commands::MacroHelpers

/*
 * use to generate the command class so that is available at runtime
 */
#define KL_DEBUG_CREATE_COMMAND(_CommandClass)                                                                                                                                                                                                                                                                     \
    namespace KL::Debug::User::Commands                                                                                                                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                                                                              \
        static_assert(THasStaticFunction_StaticGetCommandID<_CommandClass>::Value, "WARNING: Class must use the macro KL_DEBUG_COMMAND_GENERATE_WITHOUT_CONTEXT or KL_DEBUG_COMMAND_GENERATE_WITH_CONTEXT to generate both static and non static methods");                                                        \
        static_assert(!_CommandClass::StaticHasContextClass() || TIsConstructible<_CommandClass::ContextClassType, FArchive&>::Value, "WARNING: Context must have a construct that accepts FArchive so we can initialize it from network data. Add something like this explicit FYourCommandContext(FArchive&);"); \
        static inline const TKLDebugUserNetworkingCommandManagerEntry<_CommandClass> Instanced_##_CommandClass;                                                                                                                                                                                                    \
    }                                                                                                                                                                                                                                                                                                              \
                                                                                                                                                                                                                                                                                                                   \
    void _CommandClass::Client_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Client& _Input, FArchive& _ArchiveContext) const                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                                                                              \
        KL::Debug::User::Commands::MacroHelpers::ExecuteCommand<_CommandClass, ContextClassType>(*this, _Input, _ArchiveContext);                                                                                                                                                                                  \
    }                                                                                                                                                                                                                                                                                                              \
                                                                                                                                                                                                                                                                                                                   \
    void _CommandClass::Server_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Server& _Input, FArchive& _ArchiveContext) const                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                                                                              \
        KL::Debug::User::Commands::MacroHelpers::ExecuteCommand<_CommandClass, ContextClassType>(*this, _Input, _ArchiveContext);                                                                                                                                                                                  \
    }

/*
 * used inside the Command .h file to define the methods that allow us to identify this particular
 * command. _CommandID can be as simple as the command class name. For example, if your command class
 * is called FCommandClassTest, then you can pass to this macro the name FCommandClassTest
 */
#define KL_DEBUG_COMMAND_INTERNAL(_CommandID, _ContextClass)                                                                       \
public:                                                                                                                            \
    UE_NODISCARD inline static const FName& StaticGetCommandID()                                                                   \
    {                                                                                                                              \
        static const FName CommandID(TEXT(#_CommandID));                                                                           \
        return CommandID;                                                                                                          \
    }                                                                                                                              \
                                                                                                                                   \
    UE_NODISCARD inline const FName& GetID() const override                                                                        \
    {                                                                                                                              \
        return StaticGetCommandID();                                                                                               \
    }                                                                                                                              \
                                                                                                                                   \
    using ContextClassType = _ContextClass;                                                                                        \
                                                                                                                                   \
    UE_NODISCARD inline static constexpr bool StaticHasContextClass()                                                              \
    {                                                                                                                              \
        return std::is_same_v<ContextClassType, void> == false;                                                                    \
    }                                                                                                                              \
                                                                                                                                   \
    void Client_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Client& _Input, FArchive& _ArchiveContext) const override; \
    void Server_Execute(const FKLDebugUserNetworkingCommandExecuteInput_Server& _Input, FArchive& _ArchiveContext) const override; \
                                                                                                                                   \
private:

/*
 * used inside the Command .h file to define the methods that allow us to identify this particular
 * command. _CommandID can be as simple as the command class name. For example, if your command class
 * is called FCommandClassTest, then you can pass to this macro the name FCommandClassTest
 */
#define KL_DEBUG_COMMAND_GENERATE_WITHOUT_CONTEXT(_CommandID) \
    KL_DEBUG_COMMAND_INTERNAL(_CommandID, void)

/*
 * this allows to generate a context class which will contain the data that
 * will be synced over the network so the command can work properly
 * also in remote machines
 */
#define KL_DEBUG_COMMAND_GENERATE_WITH_CONTEXT(_CommandID, _ContextClass) \
    KL_DEBUG_COMMAND_INTERNAL(_CommandID, _ContextClass)
