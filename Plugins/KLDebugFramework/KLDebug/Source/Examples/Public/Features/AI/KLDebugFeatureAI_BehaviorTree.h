// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui user module
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"

class FKLDebugFeatureAI_BehaviorTreeNetworkContext;
class FKLDebugImGuiFeature_NetworkingGatherDataInput;
class FKLDebugImGuiFeature_NetworkingReceiveDataInput;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTree final : public IKLDebugImGuiFeatureInterface_Selectable, public IKLDebugImGuiFeature_NetworkingInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureAI_BehaviorTree, IKLDebugImGuiFeatureInterface_Selectable)

public:
    // IKLDebugImGuiFeatureInterface_Selectable
    UE_NODISCARD TUniquePtr<FKLDebugImGuiFeatureContext_Base> GetFeatureContext(const FKLDebugImGuiFeatureContextInput& _Input) const final;
    // IKLDebugImGuiFeatureInterface_Selectable

    // IKLDebugImGuiNetworing_FeatureInterface
    UE_NODISCARD bool Client_InformServerWhenActive() const final;
    UE_NODISCARD bool ShouldGatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const final;
    void GatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const final;
    void ReceiveData(const FKLDebugImGuiFeature_NetworkingReceiveDataInput& _Input) final;
    UE_NODISCARD bool ShouldCompressData() const final;
    // IKLDebugImGuiNetworing_FeatureInterface

private:
    // IKLDebugImGuiFeatureInterface_Selectable
    void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName& GetImGuiPath() const final;
    // IKLDebugImGuiFeatureInterface_Selectable

    void ImGuiDrawBrainInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
    void ImGuiDrawBTInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
};

inline bool FKLDebugFeatureAI_BehaviorTree::Client_InformServerWhenActive() const
{
    return true;
}

inline bool FKLDebugFeatureAI_BehaviorTree::ShouldCompressData() const
{
    return true;
}