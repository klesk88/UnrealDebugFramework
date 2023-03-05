#include "Feature/Container/ObjectSubsystem/KLDebugImGuiFeatureContainer_ObjectSubsystem.h"

FKLDebugImGuiFeatureContainer_ObjectSubsystem::FKLDebugImGuiFeatureContainer_ObjectSubsystem(const EObjectSubsytemType _Type)
    : mType(_Type)
{
}

bool FKLDebugImGuiFeatureContainer_ObjectSubsystem::IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const
{
    return _Feature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_ObjectSubsystem>()
        && reinterpret_cast<const IKLDebugImGuiFeatureInterface_ObjectSubsystem&>(_Feature).GetType() == mType;
}

void FKLDebugImGuiFeatureContainer_ObjectSubsystem::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    _OutFeaturesIndexes.Reserve(GetFeaturesCount());

    for (const uint32 FeatureOffset : GetFeaturesOffset())
    {
        const IKLDebugImGuiFeatureInterface_ObjectSubsystem& Feature = reinterpret_cast<const IKLDebugImGuiFeatureInterface_ObjectSubsystem&>(GetFeaturesPool()[FeatureOffset]);
        if (Feature.DoesSupportObject(_Obj))
        {
            _OutFeaturesIndexes.Emplace(FeatureOffset);
        }
    }

    _OutFeaturesIndexes.Shrink();
}
