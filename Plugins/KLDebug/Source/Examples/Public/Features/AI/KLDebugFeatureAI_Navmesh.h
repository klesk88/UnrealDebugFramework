// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"

// engine
#include "AITypes.h"
#include "Math/Vector.h"
#include "NavMesh/NavMeshRenderingComponent.h"

class APlayerController;
class FArchive;
class FKLDebugImGuiFeatureCanvasInput;
class FKLDebugImGuiFeature_NetworkingGatherDataInput;
class FKLDebugImGuiFeature_NetworkingReceiveDataInput;
class FKLDebugImGuiFeatureImGuiInput_Selectable;
class UCanvas;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_Navmesh final : public IKLDebugImGuiFeatureInterface_Unique, public IKLDebugImGuiFeature_NetworkingInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureAI_Navmesh, IKLDebugImGuiFeatureInterface_Unique)

public:
    // IKLDebugImGuiFeatureInterface_Unique
    UE_NODISCARD bool DoesSupportWorld(const UWorld& _Object) const final;
    void Render(const FKLDebugImGuiFeatureRenderInput_Unique& _Input) const final;
    UE_NODISCARD bool RequireCanvasUpdate() const final;
    void DrawOnCanvas(FKLDebugImGuiFeatureCanvasInput& _Input) const final;

    void OnFeatureSelected(const UWorld& _World) final;
    void OnFeatureUnselected(const UWorld& _World) final;
    // IKLDebugImGuiFeatureInterface_Unique

    // IKLDebugImGuiNetworing_FeatureInterface
    UE_NODISCARD bool ShouldGatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const final;
    void GatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const final;
    void ReceiveData(const FKLDebugImGuiFeature_NetworkingReceiveDataInput& _Input) final;
    // IKLDebugImGuiNetworing_FeatureInterface

private:
    // IKLDebugImGuiFeatureInterface_Selectable
    void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    UE_NODISCARD const FName& GetImGuiPath() const final;
    // IKLDebugImGuiFeatureInterface_Selectable

    void CollectNavmeshData(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input);

private:
    FNavMeshSceneProxyData mNavmeshRenderData;
    FVector mPlayerCameraLocation = FAISystem::InvalidLocation;
    uint32 mTilesCount = 3;
    float mMinDistanceForUpdate = 50.f;
};

inline bool FKLDebugFeatureAI_Navmesh::DoesSupportWorld([[maybe_unused]] const UWorld& _Object) const
{
    return true;
}

inline bool FKLDebugFeatureAI_Navmesh::RequireCanvasUpdate() const
{
    return true;
}