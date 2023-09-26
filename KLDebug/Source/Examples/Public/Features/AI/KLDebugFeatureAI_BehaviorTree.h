#pragma once

// imgui module
#include "ImGui/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
//ImGuiNetworking module
#include "ImGuiNetworking/Runtime/Public/Interface/KLDebugImGuiNetworking_FeatureInterface.h"

class FKLDebugFeatureAI_BehaviorTreeNetworkContext;
class FKLDebugImGuiNetworking_GatherDataInput;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTree final : public IKLDebugImGuiFeatureInterface_SelectableObject, public IKLDebugImGuiNetworking_FeatureInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureAI_BehaviorTree, IKLDebugImGuiFeatureInterface_SelectableObject)

public:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    UE_NODISCARD TUniquePtr<FKLDebugImGuiFeatureContext_Base> GetFeatureContext(const FKLDebugImGuiFeatureContextInput& _Input) const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

    //IKLDebugImGuiNetworing_FeatureInterface
    UE_NODISCARD bool Client_InformServerWhenActive() const final;
    UE_NODISCARD bool ShouldGatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const final;
    void GatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const final;
    //IKLDebugImGuiNetworing_FeatureInterface

private:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    void                        DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void                        GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName&   GetImGuiPath() const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

    void ImGuiDrawBrainInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
    void ImGuiDrawBTInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
};

inline bool FKLDebugFeatureAI_BehaviorTree::Client_InformServerWhenActive() const
{
    return true;
}
