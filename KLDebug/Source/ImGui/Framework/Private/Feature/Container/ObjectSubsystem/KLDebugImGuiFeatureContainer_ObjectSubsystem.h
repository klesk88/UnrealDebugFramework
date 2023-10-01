#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_ObjectSubsystem.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureContainer_ObjectSubsystem final : public FKLDebugImGuiFeatureContainerBase
{
public:
    explicit FKLDebugImGuiFeatureContainer_ObjectSubsystem(const EObjectSubsytemType _Type);

    // FKLDebugImGuiFeatureContainerBase
    UE_NODISCARD bool IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const final;
    void              GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const final;
    // FKLDebugImGuiFeatureContainerBase

private:
    EObjectSubsytemType mType;
};
