// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Input/Definitions/KLDebugUserInputDefinitions.h"
#include "Input/Delegates/KLDebugUserInputDelegatesClasses.h"

// engine
#include "Delegates/DelegateCombinations.h"
#include "HAL/Platform.h"

enum class EKLDebugInputReply : uint8
{
    Handled = 0,    // in this case we consider the input handled and no plugin code will be executed
    Unhandled       // we will execute the plugin code coming with this type of input
};

DECLARE_DELEGATE_RetVal_OneParam(EKLDebugInputReply, FOnKLDebugInputTrigger, const KL::Debug::User::Gameplay::Input::FOnInputTriggerData& /*_Data*/);

namespace KL::Debug::User::Gameplay::Input
{
    KLDEBUGUSERFRAMEWORK_API void BindOnInputPressedPressed(const EKLDebugGameplayInputType _Input, const FOnKLDebugInputTrigger& _Delegate);
    KLDEBUGUSERFRAMEWORK_API void UnbindOnInputPressedPressed(const EKLDebugGameplayInputType _Input);
    KLDEBUGUSERFRAMEWORK_API UE_NODISCARD EKLDebugInputReply BroadcastOnInputPressed(const EKLDebugGameplayInputType _Input, const FOnInputTriggerData& _Data);

}    // namespace KL::Debug::User::Gameplay::Input
