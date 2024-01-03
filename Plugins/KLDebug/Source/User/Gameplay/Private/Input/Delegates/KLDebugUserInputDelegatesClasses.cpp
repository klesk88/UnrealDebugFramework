// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Input/Delegates/KLDebugUserInputDelegatesClasses.h"

namespace KL::Debug::User::Gameplay::Input
{
    FOnInputTriggerData::FOnInputTriggerData(const UWorld& _World)
        : mWorld(_World)
    {
    }
}    // namespace KL::Debug::User::Gameplay::Input