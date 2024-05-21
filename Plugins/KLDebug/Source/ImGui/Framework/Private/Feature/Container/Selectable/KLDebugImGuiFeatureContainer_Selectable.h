// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Filter/Tree/KLDebugImGuiFilterTree.h"

// debug utils module
#include "Utils/Public/KLDebugLog.h"

class UObject;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureContainer_Selectable final : public FKLDebugImGuiFeatureContainerBase
{
public:
    // FKLDebugImGuiFeatureContainerBase
    [[nodiscard]] bool IsCorrectContainerForFeature(const IKLDebugFeatureInterfaceBase& _Feature) const final;
    [[nodiscard]] EImGuiInterfaceType GetContainerType() const final;
    void GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const final;
    // FKLDebugImGuiFeatureContainerBase

private:
    // FKLDebugImGuiFeatureContainerBase
    void FinishGenerateFeatureChild() final;
    // FKLDebugImGuiFeatureContainerBase

private:
    FKLDebugImGuiFilterTree mFilterTree;
};

inline EImGuiInterfaceType FKLDebugImGuiFeatureContainer_Selectable::GetContainerType() const
{
    return EImGuiInterfaceType::SELECTABLE;
}