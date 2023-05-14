#include "Feature/Container/EngineSubsystem/KLDebugImGuiFeatureContainer_EngineSubsystem.h"

#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_EngineSubsystem.h"

//engine
#include "Math/NumericLimits.h"

bool FKLDebugImGuiFeatureContainer_EngineSubsystem::IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _DumbFeature) const
{
    return _DumbFeature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_EngineSubsystem>();
}

void FKLDebugImGuiFeatureContainer_EngineSubsystem::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    checkf(GetFeaturesCount() < TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max(), TEXT("too many elements"));

    _OutFeaturesIndexes.Reserve(GetFeaturesCount());
    for (int32 i = 0; i < GetFeaturesCount(); ++i)
    {
        _OutFeaturesIndexes.Emplace(static_cast<KL::Debug::ImGui::Features::Types::FeatureIndex>(i));
    }
}
