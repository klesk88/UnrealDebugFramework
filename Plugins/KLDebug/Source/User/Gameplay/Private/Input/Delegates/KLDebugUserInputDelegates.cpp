// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Input/Delegates/KLDebugUserInputDelegates.h"

// engine
#include "Containers/StaticArray.h"

namespace KL::Debug::User::Gameplay::Input
{
    ////////////////////////////////////////////////////////
    /// private

    TStaticArray<FOnKLDebugInputTrigger, static_cast<int32>(EKLDebugGameplayInputType::Count)> OnInputTriggerDelegates;

    ////////////////////////////////////////////////////////
    /// public

    void BindOnInputPressedPressed(const EKLDebugGameplayInputType _Input, const FOnKLDebugInputTrigger& _Delegate)
    {
        ensureMsgf(!OnInputTriggerDelegates[static_cast<int32>(_Input)].IsBound(), TEXT("only one delegate at a time allowed"));

        OnInputTriggerDelegates[static_cast<int32>(_Input)] = _Delegate;
    }

    void UnbindOnInputPressedPressed(const EKLDebugGameplayInputType _Input)
    {
        OnInputTriggerDelegates[static_cast<int32>(_Input)].Unbind();
    }

    EKLDebugInputReply BroadcastOnInputPressed(const EKLDebugGameplayInputType _Input, const FOnInputTriggerData& _Data)
    {
        if (OnInputTriggerDelegates[static_cast<int32>(_Input)].IsBound())
        {
            return OnInputTriggerDelegates[static_cast<int32>(_Input)].Execute(_Data);
        }

        return EKLDebugInputReply::Unhandled;
    }

}    // namespace KL::Debug::User::Gameplay::Input