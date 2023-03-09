#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

class FKLDebugImGuiFeatureManagerEntryBase;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureContainer_EngineSubsystem final : public FKLDebugImGuiFeatureContainerBase
{
public:
    // FKLDebugImGuiFeatureContainerBase
    UE_NODISCARD bool IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _DumbFeature) const final;
    void              GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const final;
    // FKLDebugImGuiFeatureContainerBase
};