#include "Tick/KLDebugUtilsTickObject.h"

void FKLDebugUtilsTickObject::ExecuteTick(float _DeltaTime, ELevelTick _TickType, ENamedThreads::Type _CurrentThread, const FGraphEventRef& _CompletionGraphEvent)
{
    mOnTick.ExecuteIfBound(_DeltaTime);
}
