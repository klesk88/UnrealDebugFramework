// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "DebugRenderSceneProxy.h"
#include "GenericPlatform/GenericPlatform.h"
#include "PrimitiveViewRelevance.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class FMeshElementCollector;
class FSceneView;
class FSceneViewFamily;
class FStaticPrimitiveDrawInterface;
class UPrimitiveComponent;

// based of FGameplayDebuggerCompositeSceneProxy
class KLDEBUGIMGUIFRAMEWORK_API FKLDebugFrameworkRenderingCompositeSceneProxy final : public FDebugRenderSceneProxy, public FNoncopyable
{
public:
    explicit FKLDebugFrameworkRenderingCompositeSceneProxy(const UPrimitiveComponent& _Component);

    // FDebugRenderSceneProxy
    void DrawStaticElements(FStaticPrimitiveDrawInterface* _PDI) final;
    void GetDynamicMeshElements(const TArray<const FSceneView*>& _Views, const FSceneViewFamily& _ViewFamily, uint32 _VisibilityMap, FMeshElementCollector& _Collector) const final;
    FPrimitiveViewRelevance GetViewRelevance(const FSceneView* _View) const final;
    uint32 GetMemoryFootprint(void) const final;
    // FDebugRenderSceneProxy

    void AddRange(TArray<TUniquePtr<FDebugRenderSceneProxy>>&& _Children);

private:
    [[nodiscard]] uint32 GetAllocatedSizeInternal() const;

protected:
    TArray<TUniquePtr<FDebugRenderSceneProxy>> mChildProxies;
};