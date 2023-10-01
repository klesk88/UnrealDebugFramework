#include "Feature/Container/ObjectSubsystem/KLDebugImGuiFeatureContainer_ObjectSubsystem.h"

#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_ObjectSubsystem.h"

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

    for (FKLDebugImGuiFeaturesConstIterator Iterator = GetFeaturesConstIterator(); Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_ObjectSubsystem& Feature = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_ObjectSubsystem>();
        if (!Feature.DoesSupportObject(_Obj))
        {
            continue;
        }

        _OutFeaturesIndexes.Emplace(Iterator.GetFeatureDataIndex());
    }

    _OutFeaturesIndexes.Shrink();
}
