// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Helpers/KLDebugUserNetworkingCommandsRequester.h"

namespace KL::Debug::Networking::Commands::Internal
{
    ////////////////////////////////////////////////
    /// private

    static FOnKLDebugNewCommand OnNewCommandDelegate;

    ////////////////////////////////////////////////
    /// public

    void BindOnNewCommand(const FOnKLDebugNewCommand& _Delegate)
    {
        ensureMsgf(!OnNewCommandDelegate.IsBound(), TEXT("Only one delegate can be attached"));
        OnNewCommandDelegate = _Delegate;
    }

    bool CanBroadcastRequest()
    {
        return OnNewCommandDelegate.IsBound();
    }

    void BroadcastNewRequest(const FKLDebugOnNewCommandInput& _Input)
    {
        OnNewCommandDelegate.ExecuteIfBound(_Input);
    }

}    // namespace KL::Debug::Networking::Commands::Internal