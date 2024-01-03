//// Distributed under the MIT License (MIT) (see accompanying LICENSE file)
//
// #pragma once
//
//// modules
// #include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
//
//// engine
// #include "AITypes.h"
// #include "Containers/Array.h"
// #include "Engine/NetSerialization.h"
// #include "Math/Vector.h"
// #include "NavMesh/NavMeshRenderingComponent.h"
//
// class FArchive;
// class FKLDebugImGuiFeature_NetworkingGatherDataInput;
//
//// based on FGameplayDebuggerCategory_Navmesh
// class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_NavmeshNetworkContext final : public FKLDebugImGuiFeatureContext_Base
//{
//     DERIVED_KL_DEBUG_FEATURE_CONTEXT_CLASS(FKLDebugFeatureAI_NavmeshNetworkContext, FKLDebugImGuiFeatureContext_Base)
//
// public:
//     void UpdateData(const UObject& _OwnerObject);
//
// private:
//     // replicated
//     FNavMeshSceneProxyData mRepNavmeshProxyData;
//     // replicated
//
//     FVector mLastCameraUpdate = FAISystem::InvalidLocation;
// };
