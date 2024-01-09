// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Rendering/KLDebugFrameworkRenderingCompositeSceneProxy.h"

// engine
#include "SceneManagement.h"
#include "SceneView.h"

FKLDebugFrameworkRenderingCompositeSceneProxy::FKLDebugFrameworkRenderingCompositeSceneProxy(const UPrimitiveComponent& _Component)
    : FDebugRenderSceneProxy(&_Component)
{
}

void FKLDebugFrameworkRenderingCompositeSceneProxy::DrawStaticElements(FStaticPrimitiveDrawInterface* _PDI)
{
    for (const TUniquePtr<FDebugRenderSceneProxy>& DebugProxy : mChildProxies)
    {
        DebugProxy->DrawStaticElements(_PDI);
    }
}

void FKLDebugFrameworkRenderingCompositeSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& _Views, const FSceneViewFamily& _ViewFamily, uint32 _VisibilityMap, FMeshElementCollector& _Collector) const
{
    for (const TUniquePtr<FDebugRenderSceneProxy>& DebugProxy : mChildProxies)
    {
        DebugProxy->GetDynamicMeshElements(_Views, _ViewFamily, _VisibilityMap, _Collector);
    }
}

FPrimitiveViewRelevance FKLDebugFrameworkRenderingCompositeSceneProxy::GetViewRelevance(const FSceneView* _View) const
{
    FPrimitiveViewRelevance Result;
    for (const TUniquePtr<FDebugRenderSceneProxy>& DebugProxy : mChildProxies)
    {
        Result |= DebugProxy->GetViewRelevance(_View);
    }

    return Result;
}

uint32 FKLDebugFrameworkRenderingCompositeSceneProxy::GetMemoryFootprint(void) const
{
    return sizeof(*this) + GetAllocatedSizeInternal();
}

void FKLDebugFrameworkRenderingCompositeSceneProxy::AddRange(TArray<TUniquePtr<FDebugRenderSceneProxy>>&& _Children)
{
    mChildProxies.Append(MoveTemp(_Children));
}

uint32 FKLDebugFrameworkRenderingCompositeSceneProxy::GetAllocatedSizeInternal() const
{
    SIZE_T Size = FDebugRenderSceneProxy::GetAllocatedSize() + mChildProxies.GetAllocatedSize();
    for (const TUniquePtr<FDebugRenderSceneProxy>& DebugProxy : mChildProxies)
    {
        Size += DebugProxy->GetMemoryFootprint();
    }

    return IntCastChecked<uint32>(Size);
}
