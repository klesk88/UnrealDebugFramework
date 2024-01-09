// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Rendering/KLDebugFrameworkRenderingDrawDelegateHelper.h"

FKLDebugFrameworkRenderingDrawDelegateHelper::~FKLDebugFrameworkRenderingDrawDelegateHelper()
{
    Reset();
}

void FKLDebugFrameworkRenderingDrawDelegateHelper::Reset()
{
    mDebugDrawDelegateHelpers.Reset();
}

void FKLDebugFrameworkRenderingDrawDelegateHelper::RegisterDebugDrawDelegateInternal()
{
    ensureMsgf(State != RegisteredState, TEXT("DrawDelegate is already Registered!"));

    if (State == InitializedState)
    {
        for (const TUniquePtr<FDebugDrawDelegateHelper>& DelegateHelper : mDebugDrawDelegateHelpers)
        {
            DelegateHelper->RequestRegisterDebugDrawDelegate(nullptr);
        }

        State = RegisteredState;
    }
}

void FKLDebugFrameworkRenderingDrawDelegateHelper::UnregisterDebugDrawDelegate()
{
    if (State == RegisteredState)
    {
        for (const TUniquePtr<FDebugDrawDelegateHelper>& DelegateHelper : mDebugDrawDelegateHelpers)
        {
            DelegateHelper->UnregisterDebugDrawDelegate();
        }

        State = InitializedState;
    }
}
