#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureContainer_Unique final : public FKLDebugImGuiFeatureContainerBase
{
public:
    // FKLDebugImGuiFeatureContainerBase
    UE_NODISCARD bool IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const final;
    UE_NODISCARD EImGuiInterfaceType GetContainerType() const final;
    void GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const final;
    // FKLDebugImGuiFeatureContainerBase
};

inline EImGuiInterfaceType FKLDebugImGuiFeatureContainer_Unique::GetContainerType() const
{
    return EImGuiInterfaceType::UNIQUE;
}