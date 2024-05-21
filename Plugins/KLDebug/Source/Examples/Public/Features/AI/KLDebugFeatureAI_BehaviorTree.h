// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Feature/Interface/Selectable/KLDebugFeatureInterface_Selectable.h"
#include "User/Framework/Public/Networking/Feature/Selectable/KLDebugUserNetworkingFeatureSelectableInterface.h"

class FKLDebugFeatureAI_BehaviorTreeNetworkContext;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTree final : public IKLDebugFeatureInterface_Selectable, public IKLDebugUserNetworkingFeatureSelectableInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureAI_BehaviorTree, IKLDebugFeatureInterface_Selectable)
    KL_DEBUG_NETWORK_INTERFACE_FEATURE_SELECTABLE(FKLDebugFeatureAI_BehaviorTree)

public:
    // IKLDebugFeatureInterface_Selectable
    [[nodiscard]] TUniquePtr<IKLDebugContextInterface> GetContext(const FKLDebugContextGetterInput_Selectable& _Input) const final;
    // IKLDebugFeatureInterface_Selectable

    // IKLDebugUserNetworkingFeatureSelectableInterface
    [[nodiscard]] bool Server_ShouldTick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) const final;
    void Server_Tick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) final;
    void Client_ReceiveData(const FKLDebugUserNetworkingFeatureSelectableReceiveDataInput& _Input) final;
    // IKLDebugUserNetworkingFeatureSelectableInterface

private:
    // IKLDebugFeatureInterface_Selectable
    void DrawImGuiChild(FKLDebugFeatureImGuiInput_Selectable& _Input) final;
    [[nodiscard]] const FString& GetWindowName() const final;
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    [[nodiscard]] const FName& GetImGuiPath() const final;
    // IKLDebugFeatureInterface_Selectable

    void ImGuiDrawBrainInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
    void ImGuiDrawBTInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const;
};
