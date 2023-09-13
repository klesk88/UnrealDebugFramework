#include "Tick/KLDebugImGuiTickObject.h"

void FKLDebugImGuiTickObject::ExecuteTick(float _DeltaTime, ELevelTick _TickType, ENamedThreads::Type _CurrentThread, const FGraphEventRef& _CompletionGraphEvent)
{
    mOnTick.ExecuteIfBound(_DeltaTime);
}
