#pragma once

// imgui module
#include "ImGui/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

class UBehaviorTreeComponent;
class UBrainComponent;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTree : public IKLDebugImGuiFeatureInterface_SelectableObject
{
public:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    UE_NODISCARD bool DoesSupportObject(const UObject& _Object) const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

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