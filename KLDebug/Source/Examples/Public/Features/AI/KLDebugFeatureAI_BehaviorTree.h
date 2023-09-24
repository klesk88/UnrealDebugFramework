#pragma once

// imgui module
#include "ImGui/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
//ImGuiNetworkingShared module
#include "ImGuiNetworking/Runtime/Public/Interface/KLDebugImGuiNetworing_FeatureInterface.h"

class UBehaviorTreeComponent;
class UBrainComponent;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTree final : public IKLDebugImGuiFeatureInterface_SelectableObject, public IKLDebugImGuiNetworing_FeatureInterface
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureAI_BehaviorTree, IKLDebugImGuiFeatureInterface_SelectableObject)

public:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    UE_NODISCARD TUniquePtr<FKLDebugImGuiFeatureContext_Base> GetFeatureContext(const FKLDebugImGuiFeatureContextInput& _Input) const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

    //IKLDebugImGuiNetworing_FeatureInterface
    UE_NODISCARD bool Client_InformServerWhenActive() const final;
    //IKLDebugImGuiNetworing_FeatureInterface

private:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    void                        DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void                        GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName&   GetImGuiPath() const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

    UE_NODISCARD const UBehaviorTreeComponent* GetBTComponent(const UObject& _Object) const;
    UE_NODISCARD const UBrainComponent*        GetBrainComponent(const UObject& _Object) const;

    void ImGuiDrawBrainInfo(const UObject& _Object) const;
    void ImGuiDrawBTInfo(const UObject& _Object) const;
};

inline bool FKLDebugFeatureAI_BehaviorTree::Client_InformServerWhenActive() const
{
    return true;
}
