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

void FKLDebugImGuiFeatureContainer_ObjectSubsystem::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    _OutFeaturesIndexes.Reserve(GetFeaturesCount());
    const TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& FeaturePool  = GetFeaturesPool();
    const TArray<FKLDebugImGuiFeatureData>&                            FeaturesData = GetFeaturesData();

    for (int32 i = 0; i < FeaturesData.Num(); ++i)
    {
        const FKLDebugImGuiFeatureData& FeatureData = FeaturesData[i];
        checkf(FeaturePool.IsValidIndex(FeatureData.GetFeatureOffset()), TEXT("index offset must be valid"));
        const IKLDebugImGuiFeatureInterface_ObjectSubsystem& Feature = reinterpret_cast<const IKLDebugImGuiFeatureInterface_ObjectSubsystem&>(FeaturePool[FeatureData.GetFeatureOffset()]);
        if (Feature.DoesSupportObject(_Obj))
        {
            _OutFeaturesIndexes.Emplace(i);
        }
    }

    _OutFeaturesIndexes.Shrink();
}
