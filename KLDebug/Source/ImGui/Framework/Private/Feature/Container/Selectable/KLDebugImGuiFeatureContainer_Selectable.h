#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Filter/Tree/KLDebugImGuiFilterTree.h"

// debug utils module
#include "Utils/Public/KLDebugLog.h"

class UObject;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureContainer_Selectable final : public FKLDebugImGuiFeatureContainerBase
{
public:
    //FKLDebugImGuiFeatureContainerBase
    UE_NODISCARD bool IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const final;
    UE_NODISCARD EImGuiInterfaceType GetContainerType() const final;
    void              GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const final;
    //FKLDebugImGuiFeatureContainerBase

private:
    //FKLDebugImGuiFeatureContainerBase
    void FinishGenerateFeatureChild() final;
    //FKLDebugImGuiFeatureContainerBase

private:
    FKLDebugImGuiFilterTree mFilterTree;
};

inline EImGuiInterfaceType FKLDebugImGuiFeatureContainer_Selectable::GetContainerType() const
{
    return EImGuiInterfaceType::SELECTABLE;
}