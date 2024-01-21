// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Feature/Interface/Unique/KLDebugFeatureInterface_Unique.h"
#include "User/Framework/Public/Networking/Feature/Unique/KLDebugUserNetworkingFeatureUniqueInterface.h"

// engine
#include "AITypes.h"
#include "Math/Vector.h"
#include "NavMesh/NavMeshRenderingComponent.h"

class APlayerController;
class FArchive;
class UObject;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_Navmesh : public IKLDebugFeatureInterface_Unique, public IKLDebugUserNetworkingFeatureUniqueInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureAI_Navmesh, IKLDebugFeatureInterface_Unique)
    KL_DEBUG_NETWORK_INTERFACE_FEATURE_UNIQUE(FKLDebugFeatureAI_Navmesh)

public:
    // IKLDebugFeatureInterface_Unique
    UE_NODISCARD bool DoesSupportWorld(const UWorld& _Object) const final;
    void DrawOnCanvas(FKLDebugFeatureDrawCanvasInput_Unique& _Input) const final;

    void OnFeatureSelected(const UWorld& _World) final;
    void OnFeatureUnselected(const UWorld& _World) final;
    TUniquePtr<FDebugRenderSceneProxy> CreateDebugSceneProxy(FKLDebugFeatureSceneProxyInput_Unique& _Input) final;

    void Tick(FKLDebugFeatureTickInput_Unique& _Input) final;
    // IKLDebugFeatureInterface_Unique

    // IKLDebugUserNetworkingFeatureUniqueInterface
    void Server_FeatureUpdate(const FKLDebugUserNetworkingFeatureUniqueRequestUpdateInput& _Input) const final;

    void Client_Tick(FKLDebugUserNetworkingFeatureUniqueClientTickInput& _Input) final;
    void Client_ReceiveData(const FKLDebugUserNetworkingFeatureUniqueReceiveDataInput& _Input) final;
    // IKLDebugUserNetworkingFeatureUniqueInterface

private:
    // IKLDebugFeatureInterface_Selectable
    UE_NODISCARD const FString& GetWindowName() const final;
    UE_NODISCARD const FName& GetImGuiPath() const final;
    // IKLDebugFeatureInterface_Selectable

    void CollectNavmeshData(const UWorld& _World, const FVector& _Location, FNavMeshSceneProxyData& _ProxyData) const;

private:
    FNavMeshSceneProxyData mNavmeshRenderData;
    FVector mPlayerCameraLocation = FAISystem::InvalidLocation;
    uint32 mTilesCount = 3;
    float mMinDistanceForUpdate = 50.f;
    bool mRenderData = false;
};

inline bool FKLDebugFeatureAI_Navmesh::DoesSupportWorld([[maybe_unused]] const UWorld& _Object) const
{
    return true;
}
