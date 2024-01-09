// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Rendering/KLDebugFrameworkRenderingComponent.h"

#include "Rendering/KLDebugFrameworkRenderingCompositeSceneProxy.h"
#include "Rendering/KLDebugFrameworkRenderingDefinitions.h"
#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

// engine
#include "Containers/Array.h"

UKLDebugFrameworkRenderingComponent::UKLDebugFrameworkRenderingComponent(const FObjectInitializer& _ObjInitializer /*= FObjectInitializer()*/)
    : Super(_ObjInitializer)
{
}

FDebugRenderSceneProxy* UKLDebugFrameworkRenderingComponent::CreateDebugSceneProxy()
{
    UKLDebugImGuiWorldSubsystem* DebugWorldSubsystem = UKLDebugImGuiWorldSubsystem::TryGetMutable(*this);
    mDebugDrawDelegateHelper.Reset();
    if (!DebugWorldSubsystem)
    {
        ensureMsgf(false, TEXT("Must be valid"));
        return nullptr;
    }

    TArray<TUniquePtr<FDebugRenderSceneProxy>> SceneProxies;
    FKLDebugFrameworkRenderingCompositeSceneProxy* CompositeProxy = nullptr;

    auto Lambda = [this, &SceneProxies](TUniquePtr<FDebugRenderSceneProxy>&& _SceneProxy, TUniquePtr<FDebugDrawDelegateHelper>&& _DrawDelegateHelper) -> void {
        if (_SceneProxy.IsValid())
        {
            SceneProxies.Emplace(MoveTemp(_SceneProxy));
        }

        if (_DrawDelegateHelper.IsValid())
        {
            mDebugDrawDelegateHelper.AddDelegateHelper(MoveTemp(_DrawDelegateHelper));
        }
    };

    DebugWorldSubsystem->TryGatherSceneProxies(*this, Lambda);
    if (!SceneProxies.IsEmpty())
    {
        CompositeProxy = new FKLDebugFrameworkRenderingCompositeSceneProxy(*this);
        CompositeProxy->AddRange(MoveTemp(SceneProxies));
    }

    return CompositeProxy;
}
