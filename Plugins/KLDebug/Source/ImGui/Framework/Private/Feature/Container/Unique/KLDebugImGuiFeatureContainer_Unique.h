// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureContainer_Unique final : public FKLDebugImGuiFeatureContainerBase
{
public:
    // FKLDebugImGuiFeatureContainerBase
    [[nodiscard]] bool IsCorrectContainerForFeature(const IKLDebugFeatureInterfaceBase& _Feature) const final;
    [[nodiscard]] EImGuiInterfaceType GetContainerType() const final;
    void GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const final;
    // FKLDebugImGuiFeatureContainerBase
};

inline EImGuiInterfaceType FKLDebugImGuiFeatureContainer_Unique::GetContainerType() const
{
    return EImGuiInterfaceType::UNIQUE;
}