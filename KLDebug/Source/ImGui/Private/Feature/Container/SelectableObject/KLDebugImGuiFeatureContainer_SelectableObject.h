#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Filter/Tree/KLDebugImGuiFilterTree.h"

// debug utils module
#include "Utils/Public/KLDebugLog.h"

class UObject;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureContainer_SelectableObject final : public FKLDebugImGuiFeatureContainerBase
{
public:
    UE_NODISCARD bool IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const final;
    void              GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const final;

private:
    void FinishGenerateFeatureChild() final;

private:
    FKLDebugImGuiFilterTree mFilterTree;
};
