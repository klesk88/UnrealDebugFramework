#include "Input/Config/KLDebugGameplayInputConfig.h"

void FKLDebugGameplayInputConfig::Load() const
{
    MappingContext.LoadSynchronous();
    for (const TSoftObjectPtr<UInputAction>& Action : InputActions)
    {
        Action.LoadSynchronous();
    }
}
