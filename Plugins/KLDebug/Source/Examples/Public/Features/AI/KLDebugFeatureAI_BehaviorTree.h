// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "User/Networking/Public/Feature/Selectable/KLDebugUserNetworkingFeatureSelectableInterface.h"

class FKLDebugFeatureAI_BehaviorTreeNetworkContext;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTree final : public IKLDebugImGuiFeatureInterface_Selectable, public IKLDebugUserNetworkingFeatureSelectableInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureAI_BehaviorTree, IKLDebugImGuiFeatureInterface_Selectable)
    KL_DEBUG_FEATURE_NETWORK_SELECTABLE_CLASS(FKLDebugFeatureAI_BehaviorTree)

public:
    // IKLDebugImGuiFeatureInterface_Selectable
    UE_NODISCARD TUniquePtr<FKLDebugImGuiFeatureContext_Base> GetFeatureContext(const FKLDebugImGuiFeatureContextInput_Selectable& _Input) const final;
    // IKLDebugImGuiFeatureInterface_Selectable

    // IKLDebugUserNetworkingFeatureSelectableInterface
    UE_NODISCARD bool Server_ShouldTick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) const final;
    void Server_Tick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) final;
    void Client_ReceiveData(const FKLDebugUserNetworkingFeatureSelectableReceiveDataInput& _Input) final;
    // IKLDebugUserNetworkingFeatureSelectableInterface

private:
    // IKLDebugImGuiFeatureInterface_Selectable
    void DrawImGuiChild(FKLDebugImGuiFeatureImGuiInput_Selectable& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName& GetImGuiPath() const final;
    // IKLDebugImGuiFeatureInterface_Selectable

    void ImGuiDrawBrainInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
    void ImGuiDrawBTInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
};
