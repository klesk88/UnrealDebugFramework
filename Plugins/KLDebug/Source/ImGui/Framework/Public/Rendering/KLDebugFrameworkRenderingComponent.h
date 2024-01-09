// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Rendering/KLDebugFrameworkRenderingDrawDelegateHelper.h"

// engine
#include "Debug/DebugDrawComponent.h"
#include "Math/Box.h"
#include "Math/BoxSphereBounds.h"
#include "Math/TransformNonVectorized.h"
#include "Math/Vector.h"

#include "KLDebugFrameworkRenderingComponent.generated.h"

class FDebugRenderSceneProxy;

// based on UGameplayDebuggerRenderingComponent
UCLASS(ClassGroup = Debug, NotBlueprintable, NotBlueprintType, noteditinlinenew, hidedropdown, Transient)
class KLDEBUGIMGUIFRAMEWORK_API UKLDebugFrameworkRenderingComponent final : public UDebugDrawComponent
{
    GENERATED_BODY()

public:
    explicit UKLDebugFrameworkRenderingComponent(const FObjectInitializer& _ObjInitializer = FObjectInitializer());

private:
    FDebugRenderSceneProxy* CreateDebugSceneProxy() final;
    FDebugDrawDelegateHelper& GetDebugDrawDelegateHelper() final;
    FBoxSphereBounds CalcBounds(const FTransform& _LocalToWorld) const final;

private:
    FKLDebugFrameworkRenderingDrawDelegateHelper mDebugDrawDelegateHelper;
};

inline FDebugDrawDelegateHelper& UKLDebugFrameworkRenderingComponent::GetDebugDrawDelegateHelper()
{
    return mDebugDrawDelegateHelper;
}

inline FBoxSphereBounds UKLDebugFrameworkRenderingComponent::CalcBounds(const FTransform& _LocalToWorld) const
{
    static const FBoxSphereBounds DebugBounds(FBox::BuildAABB(FVector::ZeroVector, FVector(1000000.0f, 1000000.0f, 1000000.0f)));
    return DebugBounds;
}
