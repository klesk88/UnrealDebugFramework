#include "Input/Config/KLDebugGameplayInputConfig.h"

void FKLDebugGameplayInputConfig::Load() const
{
    MappingContext.LoadSynchronous();
    ToogleDebug.LoadSynchronous();
    ToogleInput.LoadSynchronous();
    ToogleDebugCamera.LoadSynchronous();
}
