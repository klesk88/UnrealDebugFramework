// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Tick/KLDebugImGuiTickObject.h"

void FKLDebugImGuiTickObject::ExecuteTick(float _DeltaTime, ELevelTick _TickType, ENamedThreads::Type _CurrentThread, const FGraphEventRef& _CompletionGraphEvent)
{
    mOnTick.ExecuteIfBound(_DeltaTime);
}
