// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "DebugRenderSceneProxy.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

// based of FGameplayDebuggerDebugDrawDelegateHelper
class KLDEBUGIMGUIFRAMEWORK_API FKLDebugFrameworkRenderingDrawDelegateHelper final : public FDebugDrawDelegateHelper, public FNoncopyable
{
public:
    ~FKLDebugFrameworkRenderingDrawDelegateHelper();

    void Reset();
    void AddDelegateHelper(TUniquePtr<FDebugDrawDelegateHelper>&& _DebugDrawDelegateHelper);

protected:
    // FDebugDrawDelegateHelper
    void RegisterDebugDrawDelegateInternal() final;
    void UnregisterDebugDrawDelegate() final;
    // FDebugDrawDelegateHelper

private:
    TArray<TUniquePtr<FDebugDrawDelegateHelper>> mDebugDrawDelegateHelpers;
};

inline void FKLDebugFrameworkRenderingDrawDelegateHelper::AddDelegateHelper(TUniquePtr<FDebugDrawDelegateHelper>&& _DebugDrawDelegateHelper)
{
    checkf(_DebugDrawDelegateHelper.IsValid(), TEXT("mustr be valid"));
    mDebugDrawDelegateHelpers.Emplace(MoveTemp(_DebugDrawDelegateHelper));
}
