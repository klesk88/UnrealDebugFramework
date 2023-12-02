// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Input/Config/KLDebugGameplayInputConfig.h"

void FKLDebugGameplayInputConfig::Load() const
{
    MappingContext.LoadSynchronous();
    for (int32 i = 0; i < static_cast<int32>(EKLDebugGameplayInputType::Count); ++i)
    {
        const TSoftObjectPtr<UInputAction>& Action = InputActions[i];
        Action.LoadSynchronous();
    }
}
