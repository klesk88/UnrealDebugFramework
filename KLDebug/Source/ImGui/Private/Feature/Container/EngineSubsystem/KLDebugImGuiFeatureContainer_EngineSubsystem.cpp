#include "Feature/Container/EngineSubsystem/KLDebugImGuiFeatureContainer_EngineSubsystem.h"

#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_EngineSubsystem.h"

bool FKLDebugImGuiFeatureContainer_EngineSubsystem::IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _DumbFeature) const
{
    return _DumbFeature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_EngineSubsystem>();
}

void FKLDebugImGuiFeatureContainer_EngineSubsystem::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    _OutFeaturesIndexes = GetFeaturesOffset();
}
